When('I configure Bugsnag for {string}') do |scenario_name|
  enter_text "Start #{scenario_name}"
end

When('I relaunch the app') do
  Maze.driver.launch_app
end

When('I run {string}') do |scenario_name|
  enter_text "Run #{scenario_name}"
end

Then('the app is not running') do
  wait_for_true do
    Maze.driver.app_state('com.bugsnag.TestFixture') == :not_running
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
