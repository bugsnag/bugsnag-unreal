$api_key = "12312312312312312312312312312312"

Before do |_scenario|
  # Wait long enough for Unreal Engine to finish loading the UI.
  # Appium appears to wait for the activity to be ready, but Unreal
  # does its own loading on a background thread.
  # TODO: can we detect when Unreal Engine has finished loading?
  sleep 5 if is_platform? 'Android'
end

Before('@not_android') do |_scenario|
  skip_this_scenario("Not compatible with Android") if is_platform? 'Android'
end

Before('@not_ios') do |_scenario|
  skip_this_scenario("Not compatible with iOS") if is_platform? 'iOS'
end

def current_platform
  case Maze.config.farm
  when :bs
    Maze.driver.capabilities['os']
  when :sl, :local
    Maze.driver.capabilities['platformName']
  else
    Maze.driver.os
  end
end

def is_platform? name
  # case-insensitive string compare, also accepts symbols. examples:
  #
  # > is_platform? :android
  # > is_platform? 'iOS'
  # > is_platform? 'ios'
  current_platform.casecmp(name.to_s) == 0
end
