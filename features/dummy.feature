Feature: Dummy

  Scenario: Crash
    When I run "Crash"
    Then the app is not running
