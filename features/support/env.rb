$api_key = "12312312312312312312312312312312"

Before do |_scenario|
  case Maze.driver.capabilities['platformName']
  when 'Android'
    # Wait long enough for Unreal Engine to finish loading the UI.
    # Appium appears to wait for the activity to be ready, but Unreal
    # does its own loading on a background thread.
    # TODO: can we detect when Unreal Engine has finished loading?
    sleep 5
  end
end
