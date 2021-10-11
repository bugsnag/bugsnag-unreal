Feature: Session tracking

  Scenario: Starting and pausing a session before a crash
    When I run "PauseSessionScenario"
    Then the app is not running

    When I relaunch the app
    And I configure Bugsnag for "PauseSessionScenario"
    And I wait to receive an error
    Then the event "session" is null

  @skip_ios
  Scenario: Resuming a session before a crash
    When I run "ResumeSessionScenario"
    Then the app is not running

    When I relaunch the app
    And I configure Bugsnag for "ResumeSessionScenario"
    And I wait to receive an error
    Then the event "session.id" is not null
