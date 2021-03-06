Feature: Session tracking

  Scenario: Starting and pausing a session before a crash
    Given I run "PauseSessionScenario" and restart the crashed app
    And I wait to receive an error
    Then the event "session" is null

  Scenario: Resuming a session before a crash
    Given I run "ResumeSessionScenario" and restart the crashed app
    And I wait to receive an error
    Then the event "session.id" is not null

  @slow
  Scenario: Cancel session from a callback
    When I run "CancelSessionScenario"
    And I wait to receive an error
    Then the error is valid for the error reporting API version "4.0" for the "Unreal Bugsnag Notifier" notifier
    And the event "session" is null
    And I should receive no sessions

  Scenario: Modify session in a callback
    When I run "ModifySessionScenario"
    And I wait to receive an error
    And I wait to receive a session
    Then the error is valid for the error reporting API version "4.0" for the "Unreal Bugsnag Notifier" notifier
    And the event "session.id" equals "4a"
    And the event "session.startedAt" equals "1976-01-03T00:00:00.000Z"
    And the event "session.events.handled" equals 1
    And the event "session.events.unhandled" equals 0
    And the session "id" equals "4a"
    And the session "startedAt" equals "1976-01-03T00:00:00.000Z"
    And the session payload field "app.type" equals "variant 2"
    And the session payload field "device.osName" equals "Fun OS"
    And the session payload field "device.osVersion" equals "3.1"
    And the session payload field "device.runtimeVersions.libpastry" equals "4.5.0.0"
    And the session payload field "device.runtimeVersions.libsauce" equals "1.0-beta_1"
    And on Android, the session payload field "app.versionCode" equals 21
    And on Android, the session payload field "device.cpuAbi" is an array with 1 elements
    And on Android, the session payload field "device.cpuAbi.0" equals "beanmachine"

  Scenario: Automatic session tracking
    When I run "AutoSessionScenario"
    And I wait to receive a session
    Then the session is valid for the session reporting API version "1.0" for the "Unreal Bugsnag Notifier" notifier

  @slow
  Scenario: Automatic session tracking disabled
    When I run "AutoTrackSessionsDisabledScenario"
    And I background the app for 3 seconds
    Then I should receive no sessions
