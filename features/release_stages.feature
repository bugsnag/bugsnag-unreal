Feature: Release stages

  @slow
  Scenario: Disabled release stage
    Given I run "ReleaseStageDisabledScenario"
    Then I should receive no errors

  Scenario: Enabled release stage
    Given I run "ReleaseStageEnabledScenario"
    And I wait to receive an error
    Then the exception "message" equals "This error should be reported"
