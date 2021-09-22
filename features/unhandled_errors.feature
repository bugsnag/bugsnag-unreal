Feature: Unhandled errors

  Scenario: BadMemoryAccessScenario
    When I run "BadMemoryAccessScenario"
    And the app is not running
    And I relaunch the app
    And I configure Bugsnag for "BadMemoryAccessScenario"
    And I wait to receive an error
    Then the error is valid for the error reporting API version "4.0" for the "Unreal Bugsnag Notifier" notifier
    And the event "unhandled" is true
    And the error payload field "notifier.dependencies.0.name" is not null
    And the error payload field "notifier.dependencies.0.url" is not null
    And the error payload field "notifier.dependencies.0.version" is not null
    And the event has a "state" breadcrumb named "Bugsnag loaded"
    And the event has a "manual" breadcrumb named "About to read from a bad memory address"

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
    # pending PLAT-7305
    # And the event "app.context" equals "Main Menu"
