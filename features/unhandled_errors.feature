Feature: Unhandled errors

  Scenario: BadMemoryAccessScenario
    When I run "BadMemoryAccessScenario"
    And the app is not running
    And I relaunch the app
    And I configure Bugsnag for "BadMemoryAccessScenario"
    And I wait to receive an error
    Then the error is valid for the error reporting API version "4.0" for the "Unreal Bugsnag Notifier" notifier
    And the error payload field "notifier.dependencies.0.name" is not null
    And the error payload field "notifier.dependencies.0.url" is not null
    And the error payload field "notifier.dependencies.0.version" is not null
    And the event "unhandled" is true
    # TODO: pending on iOS (PLAT-7313)
    And on Android, the event "context" equals "overhead view"
    And the event has a "state" breadcrumb named "Bugsnag loaded"
    And the event has a "manual" breadcrumb named "About to read from a bad memory address"
    And the event "app.isLaunching" is true
    And the event "metaData.pastries.cronut" is false
    And the event "metaData.pastries.macaron" equals 3
    And the event "metaData.counters.forty" equals "40"
    And the event "metaData.counters.thirty-five" equals "35"
    # TODO: pending on iOS
    And on Android, the event "user.id" equals "5402"
    And on Android, the event "user.email" equals "usr@example.com"
    And on Android, the event "user.name" is null
    # TODO: pending on Android (PLAT-7364)
    And on iOS, the event "metaData.custom.configOnSendError" equals "hello"
    And on iOS, the event "metaData.custom.someValue" equals "foobar"
    And on iOS, the event "metaData.device.adapterName" is not null
    # And on Android, the event "metadata.device.driverVersion" is not null
    And on iOS, the event "metaData.unrealEngine.mapUrl" matches "/Game/MainLevel"
    And on iOS, the event "metaData.unrealEngine.gameStateName" equals "GameStateBase"
    And on iOS, the event "metaData.unrealEngine.userActivity" is not null
    And on iOS, the event "metaData.unrealEngine.version" matches "\d\.\d+\.\d+-\d+"
    # TODO: pending on Android (PLAT-7364, PLAT-7367, PLAT-7369)
    And on iOS, the event "metaData.lastRunInfo.consecutiveLaunchCrashes" equals 1
    And on iOS, the event "metaData.lastRunInfo.crashed" is true
    And on iOS, the event "metaData.lastRunInfo.crashedDuringLaunch" is true
    And the method of stack frame 0 is equivalent to "BadMemoryAccessScenario::Run()"
    And the exception "errorClass" equals the platform-dependent string:
      | android | SIGSEGV |
      | ios | EXC_BAD_ACCESS |
    And the exception "message" equals the platform-dependent string:
      | android | Segmentation violation (invalid memory reference) |
      | ios | Attempted to dereference null pointer. |
    And the exception "type" equals the platform-dependent string:
      | android | c |
      | ios | cocoa |

  Scenario: Crash after setting optional configuration options
    When I run "MaxConfigCrashScenario"
    Then the app is not running
    When I relaunch the app
    And I configure Bugsnag for "MaxConfigCrashScenario"
    And I wait to receive an error
    Then the error payload field "apiKey" equals "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    And the event "unhandled" is true
    And the event "app.version" equals "2.61.0"
    And the event "app.type" equals "mailer"
    And the event "app.releaseStage" equals "beta04"
    And the event "user.id" equals "1255"
    And the event "user.name" equals "jimmy"
    And the event "user.email" equals "j@example.com"
    And on iOS, the event "app.bundleVersion" equals "2.61.0.1"
    And on Android, the event "app.versionCode" equals 22
    # TODO: pending on Android (PLAT-7305)
    And on iOS, the event "context" equals "Main Menu"
    And the method of stack frame 0 is equivalent to "MaxConfigCrashScenario::Run()"
