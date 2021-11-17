require 'rbconfig'

HOST_OS = RbConfig::CONFIG['host_os']

When('I relaunch the app') do
  Maze.driver.launch_app
  sleep(3) if is_platform? 'Android'
end

When('I run {string}') do |scenario_name|
  dial_number_for scenario_name
  tap_button 10
end

When('I configure Bugsnag for {string}') do |scenario_name|
  dial_number_for scenario_name
  tap_button 11
end

When('I run {string} and restart the crashed app') do |scenario_name|
  steps %(
    Given I run "#{scenario_name}"
    And the app is not running
    And I relaunch the app
    And I configure Bugsnag for "#{scenario_name}"
  )
end

def dial_number_for(scenario_name)
  number = $scenario_names.index scenario_name
  raise "Scenario name #{scenario_name} is not in the list; try running update-scenario-names.sh" if number.nil?
  "#{number}".each_char do |button_number|
    tap_button button_number.to_i
  end
end

def tap_button(button_number)
  row_count = 13 # 0-9, run, start, text box
  button_height = window_height / row_count
  y = (button_height * button_number) + (button_height / 2)
  Appium::TouchAction.new.tap({:x => 50, :y => y}).perform
  sleep 0.5
end

def window_height
  if is_platform? 'Android'
    Maze.driver.window_size['height'] + Maze.driver.get_system_bars['navigationBar']['height']
  else
    Maze.driver.window_size['height']
  end
end

Then('the app is running') do
  wait_for_true do
    state = app_state()
    $logger.info "app state: #{state}"
    state == :running_in_foreground
  end
end

Then('the app is not running') do
  wait_for_true do
    state = app_state()
    # workaround for faulty app state detection in appium v1.23 and lower on
    # Android where an app that is not running is detected to be running in
    # the background
    state == :not_running || (state == :running_in_background && is_platform?('Android'))
  end
end

Then(/^on (Android|iOS), (.+)/) do |platform, step_text|
  step(step_text) if is_platform? platform
end

Then('the method of stack frame {int} is equivalent to {string}') do |frame_index, method|
  assert_equal(method, parse_method(frame_index))
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
    cmd = HOST_OS.start_with?('darwin') ? 'xcrun objdump' : 'llvm-objdump-11'
    `#{cmd} --arch arm64 --syms --stop-address 0x#{stop_addr.to_s(16)} --start-address 0x#{start_addr.to_s(16)} #{dsym_path} | tail -n 1 | awk '{print $5;}' | c++filt --strip-underscore`.chomp
  else
    stackframe_method = Maze::Helper.read_key_path(
      Maze::Server.errors.current[:body],
      "events.0.exceptions.0.stacktrace.#{frame_index}.method")
    `c++filt --strip-underscore '_#{stackframe_method}'`.chomp
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
