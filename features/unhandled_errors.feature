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
    And on Android, the event "context" equals "overhead view"
    And the event has a "state" breadcrumb named "Bugsnag loaded"
    And the event has a "manual" breadcrumb named "About to read from a bad memory address"
    And the event "metaData.pastries.cronut" is false
    And the event "metaData.pastries.macaron" equals 3
    And the event "metaData.counters.forty" equals "40"
    And the event "metaData.counters.thirty-five" equals "35"
    And the method of stack frame 0 is equivalent to "BadMemoryAccessScenario::Run()"
    And the error payload field "events.0.exceptions.0.errorClass" equals the platform-dependent string:
      | android | SIGSEGV |
      | ios | EXC_BAD_ACCESS |
    And the error payload field "events.0.exceptions.0.message" equals the platform-dependent string:
      | android | Segmentation violation (invalid memory reference) |
      | ios | Attempted to dereference null pointer. |
    And the error payload field "events.0.exceptions.0.type" equals the platform-dependent string:
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
    And on iOS, the event "app.context" equals "Main Menu"
    And the method of stack frame 0 is equivalent to "MaxConfigCrashScenario::Run()"

