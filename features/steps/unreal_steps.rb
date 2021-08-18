When('I run {string}') do |input|
  # Unreal Engine doesn't support screen readers on Android, so the test fixture consists
  # of a full-screen text field (on all platforms.)
  Appium::TouchAction.new.tap({:x => 100, :y => 100}).perform

  # Wait for keyboard to appear
  sleep 1

  # '$' allows our OnTextChanged function to detect the end of text input.
  Maze.driver.driver.action.send_keys("#{input}$").perform

  case Maze.driver.capabilities['platformName']
  when 'iOS'
    # On iOS, tapping an EditableText causes an alert with text field to be presented, and
    # the EditableText is only updated when the OK button is tapped.
    Maze.driver.click_element("OK")

    # For some reason when running on a locally connected iOS device, the send_keys action
    # can end up dismissing the alert via its Cancel button, making this unreliable :-(
  end
end

Then('the app is not running') do
  wait_for_true do
    Maze.driver.app_state('com.bugsnag.TestFixture') == :not_running
  end
end

def wait_for_true
  attempts = 0
  assertion_passed = false
  until (attempts >= 5) || assertion_passed
    attempts += 1
    assertion_passed = yield
    sleep 1
  end
  raise 'Assertion not passed within 5 seconds' unless assertion_passed
end
