require 'rbconfig'

HOST_OS = RbConfig::CONFIG['host_os']

When('I relaunch the app') do
  Maze.driver.launch_app
  sleep(3) if is_platform? 'Android'
end

When('I run {string}') do |scenario_name|
  case scenario_name
  when 'BadMemoryAccessScenario'
    press_button 0
  when 'NotifyScenario'
    press_button 2
  else
    raise "Scenario #{scenario_name} not yet implemented"
  end
end

When('I configure Bugsnag for {string}') do |scenario_name|
  case scenario_name
  when 'BadMemoryAccessScenario'
    press_button 1
  else
    raise "Scenario #{scenario_name} not yet implemented"
  end
end

def press_button(button_number)
  design_height = 1200
  design_button_height = 100
  window_height = Maze.driver.window_size['height']

  x = scaled_x = 50
  y = (design_button_height * button_number) + design_button_height
  scaled_y = y * window_height / design_height

  $logger.debug "Press at: #{scaled_x},#{scaled_y} (scaled from #{x},#{y})"

  touch_action = Appium::TouchAction.new
  touch_action.tap({:x => scaled_x, :y => scaled_y})
  touch_action.perform
  sleep 1
end

Then('the app is running') do
  wait_for_true do
    $logger.info Maze.driver.app_state('com.bugsnag.TestFixture')
    Maze.driver.app_state('com.bugsnag.TestFixture') == :running_in_foreground
  end
end

Then('the app is not running') do
  wait_for_true do
    Maze.driver.app_state('com.bugsnag.TestFixture') == :not_running
  end
end

Then(/^on (Android|iOS), (.+)/) do |platform, step_text|
  steps(step_text) if is_platform? platform
end

Then('the method of stack frame {int} is equivalent to {string}') do |frame_index, method|
  # Revisit post-CI merge, needs c++filt and llvm-objdump installed on linux
  if Maze.config.farm == :local
    assert_equal(method, parse_method(frame_index))
  else
    puts "Skipping step due to missing tooling ..."
  end
end

def parse_method frame_index
  if is_platform? 'iOS'
    # Assumes this is resolving a symbol from the app binary (instead of
    # system frameworks, other bundled executables, etc)
    stackframe = Maze::Helper.read_key_path(
      Maze::Server.errors.current[:body],
      "events.0.exceptions.0.stacktrace.#{frame_index}")
    dsym_path = File.join(artifact_path, 'TestFixture-IOS-Shipping.dSYM')
    stop_addr = Integer(stackframe["frameAddress"]) - Integer(stackframe["machoLoadAddress"]) + Integer(stackframe["machoVMAddress"])
    start_addr = stop_addr - 4096
    cmd = HOST_OS == 'darwin' ? 'xcrun objdump' : 'llvm-objdump'
    `#{cmd} --arch arm64 --syms --stop-address 0x#{stop_addr.to_s(16)} --start-address 0x#{start_addr.to_s(16)} #{dsym_path} | tail -n 1 | awk '{print $5;}' | c++filt -_`.chomp
  else
    stackframe_method = Maze::Helper.read_key_path(
      Maze::Server.errors.current[:body],
      "events.0.exceptions.0.stacktrace.#{frame_index}.method")
    `c++filt '_#{stackframe_method}'`.chomp
  end
end

def enter_text(text)
  # Unreal Engine doesn't support screen readers on Android, so the test fixture
  # consists of a full-screen text field (on all platforms.)
  Appium::TouchAction.new.tap({:x => 100, :y => 100}).perform

  # Wait for keyboard to appear
  sleep 1

  # '$' allows our OnTextChanged function to detect the end of text input.
  Maze.driver.driver.action.send_keys("#{text}$").perform

  if Maze.driver.capabilities['platformName'] == 'iOS'
    # On iOS, tapping an EditableText causes an alert with text field to be
    # presented, and the EditableText is only updated when the OK button is
    # tapped. For an unknown reason, when running on a locally connected iOS
    # device, the send_keys action dismisses the alert automatically (and
    # unfortunately prefers the Cancel button.)
    if Maze.config.farm == :bs
      Maze.driver.click_element("OK")
    end
  end
end

def wait_for_true
  attempts = 0
  assertion_passed = false
  until (attempts >= 10) || assertion_passed
    attempts += 1
    assertion_passed = yield
    sleep 1
  end
  raise 'Assertion not passed within 5 seconds' unless assertion_passed
end
