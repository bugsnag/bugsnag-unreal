require 'rbconfig'

HOST_OS = RbConfig::CONFIG['host_os']

When('I relaunch the mobile app') do
  Maze.driver.launch_app
  sleep 3
end

When('I run {string}') do |scenario_name|
  if is_platform? :macos
    run_mac_fixture(:run_scenario, scenario_name)
  else
    dial_number_for scenario_name
    tap_button 10
  end
end

When('I configure Bugsnag for {string}') do |scenario_name|
  if is_platform? :macos
    run_mac_fixture(:start_bugsnag, scenario_name)
  else
    dial_number_for scenario_name
    tap_button 11
  end
end

When('I run {string} and restart the crashed app') do |scenario_name|
  step %(I run "#{scenario_name}" and restart the crashed app for "#{scenario_name}")
end

When('I run {string} and restart the crashed app for {string}') do |scenario_1, scenario_2|
  if is_platform? :macos
    run_mac_fixture(:run_scenario, scenario_1, wait: true)
    run_mac_fixture(:start_bugsnag, scenario_2)
  else
    steps %(
      Given I run "#{scenario_1}"
      And the mobile app is not running
      And I relaunch the mobile app
      And I configure Bugsnag for "#{scenario_2}"
    )
  end
end

When('I background the app for {int} seconds') do |duration|
  if is_platform? :macos
    `osascript -e 'tell application "System Events" to tell process "TestFixture-Mac-Shipping" to set visible to false'`
    sleep duration
    `osascript -e 'tell application "TestFixture-Mac-Shipping" to activate'`
  else
    Maze.driver.background_app(duration)
  end
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

Then('the mobile app is not running') do
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

Then(/^on mobile, (.+)/) do |step_text|
  step(step_text) if is_platform?(:android) || is_platform?(:ios)
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
    dsym_path = File.join(artifact_path, 'TestFixture-IOS-Shipping*.dSYM')
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

def run_mac_fixture(action, scenario_name, wait: false)
  $fixture_pid = Process.spawn(
    'features/fixtures/generic/ArchivedBuilds/MacNoEditor/TestFixture-Mac-Shipping.app/Contents/MacOS/TestFixture-Mac-Shipping',
    '-action', action.to_s, '-scenario_name', scenario_name,
    '-windowed', '-resx=720', '-resy=1080',
    '-unattended', # Prevents "Please update to the latest version of macOS" alert on macOS < 10.15.5
    [:out, :err]=>'TestFixture-Mac-Shipping.log')
  if wait
    Process.wait $fixture_pid
    $fixture_pid = nil
  else
    # Ideally we would wait until we know the Scenario::Run() method has been executed
    sleep 1
  end
end
