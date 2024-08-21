#! /usr/bin/env ruby

require 'bumpsnag'

def update_version(file, from, to)
  initial_content = File.open(file).read
  output_content = initial_content.gsub(from, to)
  File.open(file, 'w') { |file| file.puts output_content }
end

target_submodule = ENV['TARGET_SUBMODULE']
target_version = ENV['TARGET_VERSION']

if target_submodule.nil? || target_version.nil?
  raise 'Submodule or version targets not provided, exiting'
  exit(1)
end

if target_submodule.eql?('bugsnag-android')
  version = /(\d+\.\d+\.\d+)/.match(target_version)[1]
  update_version(
    'src/Source/Bugsnag/Bugsnag_UPL.xml',
    /com.bugsnag,bugsnag-android,\d{1,3}\.\d{1,3}\.\d{1,3}/,
    "com.bugsnag,bugsnag-android,#{version}"
  )
  update_version(
    'deps/bugsnag-plugin-android-unreal/build.gradle',
    /com.bugsnag:bugsnag-android-core:\d{1,3}\.\d{1,3}\.\d{1,3}/,
    "com.bugsnag:bugsnag-android-core:#{version}"
  )
elsif target_submodule.eql?('bugsnag-cocoa')
  Bumpsnag.update_submodule('deps/bugsnag-cocoa', target_version)
else
  raise "Submodule #{target_submodule} not supported, exiting"
end


