Feature: Reporting handled errors

  @not_android
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
    And the event "app.type" equals "iOS"
    And the event "app.version" equals "1.0"
    And the event "app.version" equals "1.0"
    And the event "device.freeDisk" is not null
    And the event "device.freeMemory" is not null
    And the event "device.id" is not null
    And the event "device.jailbroken" is false
    And the event "device.locale" is not null
    And the event "device.manufacturer" is not null
    And the event "device.model" is not null
    And the event "device.modelNumber" is not null
    And the event "device.orientation" equals "portrait"
    And the event "device.osName" matches "(android|iOS)"
    And the event "device.osVersion" is not null
    And the event "device.runtimeVersions" is not null
    And the event "device.time" is not null
    And the event "device.totalMemory" is not null
    And the event "metaData.device.batteryLevel" is not null
    And the event "metaData.device.charging" is not null
    And the event "severity" equals "warning"
    And the event "severityReason.type" equals "handledError"
    And the event "unhandled" is false
    And the event has a "state" breadcrumb named "Bugsnag loaded"
    And the exception "errorClass" equals "Internal Error"
    And the exception "message" equals "Does not compute"
