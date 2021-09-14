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
