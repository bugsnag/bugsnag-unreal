Feature: Reporting handled errors

  Scenario: NotifyScenario
    When I run "NotifyScenario"
    And I wait to receive an error
    Then the error is valid for the error reporting API version "4.0" for the "Unreal Bugsnag Notifier" notifier
    And the error payload field "events.0.threads" is a non-empty array
    And the error payload field "notifier.dependencies.0.name" is not null
    And the error payload field "notifier.dependencies.0.url" is not null
    And the error payload field "notifier.dependencies.0.version" is not null
    And on Android, the event "context" equals "pause menu"
    And the event "app.duration" is not null
    And the event "app.durationInForeground" is not null
    And the event "app.id" equals "com.bugsnag.TestFixture"
    And the event "app.inForeground" is true
    And the event "app.isLaunching" is true
    And the event "app.releaseStage" equals "production"
    And the event "app.type" equals the platform-dependent string:
      | android | android |
      | ios     | iOS     |
    And the event "app.version" equals "1.0"
    And the event "device.freeDisk" is not null
    And the event "device.freeMemory" is not null
    And the event "device.id" is not null
    And the event "device.jailbroken" is false
    And the event "device.locale" is not null
    And the event "device.manufacturer" is not null
    And the event "device.model" is not null
    And on iOS, the event "device.modelNumber" is not null
    And the event "device.orientation" matches "(face(down|up)|landscape(left|right)|portrait(upsidedown)?)"
    And the event "device.osName" equals the platform-dependent string:
      | android | android |
      | ios     | iOS     |
    And the event "device.osVersion" is not null
    And the event "device.runtimeVersions" is not null
    And the event "device.time" is not null
    And the event "device.totalMemory" is not null
    And the event "metaData.device.batteryLevel" is not null
    And the event "metaData.device.charging" is not null
    And the event "metaData.pastries.cronut" is false
    And the event "metaData.pastries.macaron" equals 3
    And the event "metaData.counters.forty" equals "40"
    And the event "metaData.counters.thirty-five" equals "35"
    # TODO: pending on Android (PLAT-7364)
    And on iOS, the event "metaData.custom.configOnSendError" equals "hello"
    And on iOS, the event "metaData.custom.someValue" equals "foobar"
    And on iOS, the event "metaData.custom.notify" equals "testing"
    And on iOS, the event "metaData.device.adapterName" is not null
    # And on Android, the event "metadata.device.driverVersion" is not null
    And on iOS, the event "metaData.unrealEngine.mapUrl" matches "/Game/MainLevel"
    And on iOS, the event "metaData.unrealEngine.gameStateName" equals "GameStateBase"
    And on iOS, the event "metaData.unrealEngine.userActivity" is not null
    And on iOS, the event "metaData.unrealEngine.version" matches "\d\.\d+\.\d+-\d+"
    And the event "severity" equals "warning"
    And the event "severityReason.type" equals the platform-dependent string:
      | android | handledException |
      | ios     | handledError     |
    And the event "unhandled" is false
    And the event has a "state" breadcrumb named "Bugsnag loaded"
    And the exception "errorClass" equals "Internal Error"
    And the exception "message" equals "Does not compute"
    And the method of stack frame 0 is equivalent to "NotifyScenario::Run()"
    And the exception "type" equals the platform-dependent string:
      | android | c     |
      | ios     | cocoa |
