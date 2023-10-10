Feature: Reporting handled errors

@skip_android #PLAT-9770
  Scenario: NotifyScenario
    When I run "NotifyScenario"
    And I wait to receive an error
    Then the error is valid for the error reporting API version "4.0" for the "Unreal Bugsnag Notifier" notifier
    And the error payload field "events.0.threads" is a non-empty array
    And on Android, the error payload field "events.0.threads" is an array with 4 elements
    And the error payload field "notifier.dependencies.0.name" is not null
    And the error payload field "notifier.dependencies.0.url" is not null
    And the error payload field "notifier.dependencies.0.version" is not null
    And the event "context" starts with "Lorem ipsum dolor sit amet"
    And the event "app.duration" equals 37
    And the event "app.durationInForeground" is not null
    And the event "app.id" equals "om.bugsnag.examples.UnrealTestFixture"
    And the event "app.inForeground" is true
    And the event "app.isLaunching" is true
    And the event "app.releaseStage" equals "production"
    And the event "app.type" equals the platform-dependent string:
      | android | android |
      | ios     | iOS     |
      | macos   | macOS   |
    And the event "app.version" equals "1.0"
    And the event "device.freeDisk" is not null
    And the event "device.freeMemory" is not null
    And the event "device.id" equals "51229"
    And the event "device.jailbroken" is false
    And the event "device.locale" is not null
    And the event "device.manufacturer" is not null
    And the event "device.model" is not null
    And on iOS, the event "device.modelNumber" is not null
    And on mobile, the event "device.orientation" matches "(face(down|up)|landscape(left|right)|portrait(upsidedown)?)"
    And the event "device.osName" equals the platform-dependent string:
      | android | android |
      | ios     | iOS     |
      | macos   | Mac OS  |
    And the event "device.osVersion" is not null
    And the event "device.runtimeVersions" is not null
    And on Android, the event "device.runtimeVersions.androidApiLevel" is not null
    And the event "device.runtimeVersions.unrealEngine" is not null
    And the event "device.time" is not null
    And the event "device.totalMemory" is not null
    And on mobile, the event "metaData.device.batteryLevel" is not null
    And on mobile, the event "metaData.device.charging" is not null
    And the event "metaData.pastries.cronut" is false
    And the event "metaData.pastries.macaron" equals 3
    And the event "metaData.counters.forty" equals "40"
    And the event "metaData.counters.thirty-five" equals "35"
    And the event "user.id" equals "1118"
    And the event "user.email" equals "emilie@example.com"
    And the event "user.name" equals "Emilie"
    And the event "metaData.custom.configOnSendError" equals "hello"
    And the event "metaData.custom.someValue" equals "foobar"
    And the event "metaData.custom.lastValue" is true
    And the event "metaData.custom.notify" equals "testing"
    And the event "metaData.device.gpuAdapterName" is not null
    And on Android, the event "metaData.device.gpuDriverVersion" is not null
    And on Android, the error payload field "events.0.projectPackages" is an array with 1 elements
    And on Android, the event "projectPackages.0" equals "com.example.package"
    And the event "metaData.unrealEngine.mapUrl" matches "/Game/MainLevel"
    And the event "metaData.unrealEngine.gameStateName" equals "GameStateBase"
    And the event "metaData.unrealEngine.userActivity" is not null
    And the event "metaData.unrealEngine.version" matches "\d\.\d+\.\d+-\d+"
    And the event "severity" equals "warning"
    And the event "severityReason.type" equals "handledError"
    And the event "unhandled" is false
    And the event contains the following feature flags:
      | featureFlag | variant        |
      | Testing     |                |
      | fc1         |                |
      | fc2         | teal           |
      | Bugsnag     |                |
      | Notify      | Internal Error |
    And the event has a "state" breadcrumb named "Bugsnag loaded"
    And the exception "errorClass" equals "Internal Error happened"
    And the exception "message" equals "Does not compute"
    And the method of stack frame 0 is equivalent to "NotifyScenario::Run()"
    And the exception "type" equals the platform-dependent string:
      | android | c     |
      | ios     | cocoa |
      | macos   | cocoa |
    And on iOS, the error payload field "events.0.exceptions.0.stacktrace.0.method" is null
    And on iOS, the error payload field "events.0.exceptions.0.stacktrace.0.symbolAddress" is not null

  Scenario: Notify after crash
    Given I run "CrashAfterLaunchedScenario" and restart the crashed app for "NotifyWithLaunchInfoScenario"
    And I wait to receive 2 errors
    Then the event "metaData.lastRunInfo" is null
    And the event "app.isLaunching" is false
    And I discard the oldest error
    And the exception "errorClass" equals "Resolution failed"
    And the exception "message" equals "invalid index (-1)"
    And the event "metaData.lastRunInfo.crashed" is true
    And the event "metaData.lastRunInfo.crashedDuringLaunch" is false
    And the event "metaData.lastRunInfo.consecutiveLaunchCrashes" equals 0

  Scenario: Notify with a callback changing a lot of things
    When I run "NotifyChangingEverythingInCallback"
    And I wait to receive an error
    Then the error is valid for the error reporting API version "4.0" for the "Unreal Bugsnag Notifier" notifier
    And the event "context" equals "changing lots"
    And the event "app.inForeground" is false
    And the event "app.isLaunching" is false
    And the event "device.id" equals "51229"
    And the event "device.jailbroken" is true
    And the event "metaData.custom.inConfigure" is null
    And the event "metaData.custom.missing" is true
    And the event "metaData.custom.number" equals 40
    And the event "metaData.custom.text" equals "some"
    And the event "severity" equals "info"
    And the event "unhandled" is true
    And the event "user.email" is null
    And the event "user.id" equals "319"
    And the event "user.name" is null
    And the exception "errorClass" equals "Why would you change this lol"
    And the exception "message" equals "Its literally a function argument"
    And the error payload field "events.0.threads" is a non-empty array
    And the error payload field "events.0.threads.0.name" equals "Why would you change this either?"
    And on iOS, the error payload field "events.0.threads.0.id" equals "9000"
    And on Android, the error payload field "events.0.threads.0.id" equals 9000

  @slow
  Scenario: Cancel notify from callback
    When I run "CancelNotifyFromCallback"
    Then I should receive no errors

  Scenario: Discard handled error via DiscardClasses
    When I run "DiscardClassesHandledErrorsScenario"
    And I wait to receive an error
    Then the exception "errorClass" equals "ImportantError"

  Scenario: Settings are honored when using Start(ApiKey)
    When I run "StartWithApiKeyScenario"
    And I wait to receive an error
    And the event "context" equals "FromSettings"

  Scenario: Multithreaded notify
    Given I run "NotifyMultithreadedScenario"
    And I wait to receive a session
    Then the session is valid for the session reporting API version "1.0" for the "Unreal Bugsnag Notifier" notifier
