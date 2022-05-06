BeforeAll do
  $api_key = "12312312312312312312312312312312"
end

Before do |_scenario|
  # Wait long enough for Unreal Engine to finish loading the UI.
  # Appium appears to wait for the activity to be ready, but Unreal
  # does its own loading on a background thread.
  # TODO: We could have the test fixture send a /log request on load,
  #   with a Background step to wait for the receipt of that.
  sleep 5 if is_platform?(:android) || is_platform?(:ios)
end

Before('@skip_android') do |_scenario|
  skip_this_scenario("Not compatible with Android") if is_platform? 'Android'
end

Before('@skip_ios') do |_scenario|
  skip_this_scenario("Not compatible with iOS") if is_platform? 'iOS'
end

def current_platform
  case Maze.config.farm
  when :bs
    Maze.driver.capabilities['os']
  when :sl, :local
    Maze.driver.capabilities['platformName']
  when :none
    Maze.config.os
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

def platform_artifact_dir
  if is_platform? 'iOS'
    'IOS' # why did they do this to us lol
  else
    current_platform.capitalize
  end
end

def artifact_path
  if Maze.config.farm == :local
    File.join(File.dirname(__FILE__), '..', 'fixtures', 'generic', 'Binaries', platform_artifact_dir)
  else
    '/app/build'
  end
end

def app_state
  Maze.driver.app_state('com.bugsnag.TestFixture')
end

Maze.hooks.before do
  $started_at = Time.now
end

Maze.hooks.after do |scenario|
  folder1 = File.join(Dir.pwd, 'maze_output')
  folder2 = scenario.failed? ? 'failed' : 'passed'
  folder3 = scenario.name.gsub(/[:"& ]/, '_').gsub(/_+/, '_')

  path = File.join(folder1, folder2, folder3)

  FileUtils.makedirs(path)

  case Maze::Helper.get_current_platform
  when 'macos'
    Process.kill('KILL', $fixture_pid) if $fixture_pid
    $fixture_pid = nil
    Process.wait(
      Process.spawn(
        '/usr/bin/log', 'show', '--predicate', 'process == "TestFixture-Mac-Shipping"',
        '--style', 'syslog', '--start', $started_at.strftime('%Y-%m-%d %H:%M:%S%z'),
        out: File.open(File.join(path, 'TestFixture-Mac-Shipping.log'), 'w')
      )
    )
  end
end
