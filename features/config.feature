Feature: Configuration options

  Scenario: Check if isStarted is true
    Given I run "RedactedKeysScenario"
    And I wait to receive an error
    Then the exception "errorClass" equals "Test"
    And the event "metaData.custom.password" equals "[REDACTED]"
    And the event "metaData.custom.auth_key" equals "[REDACTED]"
    And the event "metaData.custom.text" equals "This is not confidential"
    And the event "breadcrumbs.0.metaData.password" equals "[REDACTED]"
    # FIXME: custom redacted keys are not applied to breadcrumb on Android
    And on iOS, the event "breadcrumbs.0.metaData.secret" equals "[REDACTED]"
    And the event "breadcrumbs.0.metaData.foo" equals "bar"