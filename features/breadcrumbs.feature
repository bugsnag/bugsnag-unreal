Feature: Breadcrumbs and modifying with callbacks

  Scenario: Filtering and modifying breadcrumb contents
    When I run "ModifyCrumbsScenario"
    And I wait to receive an error
    Then the error is valid for the error reporting API version "4.0" for the "Unreal Bugsnag Notifier" notifier
    And the error payload field "events.0.breadcrumbs" is an array with 1 elements
    And the event "breadcrumbs.0.name" equals "Crash time now"
    And the event "breadcrumbs.0.type" equals "request"
    And the event "breadcrumbs.0.metaData.forty" equals "four zero"
    And the event "breadcrumbs.0.metaData.macaron" equals 3
    And the event "breadcrumbs.0.metaData.cronut" is false
    And the event "breadcrumbs.0.metaData.functionLibrary" equals "ok"

  Scenario: Filtering out all automatic breadcrumbs
    When I run "RestrictAllCrumbTypesScenario"
    And I wait to receive an error
    Then the error is valid for the error reporting API version "4.0" for the "Unreal Bugsnag Notifier" notifier
    And the error payload field "events.0.breadcrumbs" is an array with 1 elements
    And the event "breadcrumbs.0.name" equals "Message in a bottle"
    And the event "breadcrumbs.0.type" equals "navigation"

  Scenario: Filtering out some automatic breadcrumbs by type
    When I run "RestrictSomeCrumbTypesScenario"
    And I wait to receive an error
    Then the error is valid for the error reporting API version "4.0" for the "Unreal Bugsnag Notifier" notifier
    And the error payload field "events.0.breadcrumbs" is an array with 1 elements
    And the event "breadcrumbs.0.name" equals "Message in a bottle"
    And the event "breadcrumbs.0.type" equals "manual"

  Scenario: Automatic breadcrumbs for Unreal Engine map changes
    When I run "OpenLevelBreadcrumbsScenario"
    And I wait to receive an error
    And the event "breadcrumbs.0.name" equals "Map Loading"
    And the event "breadcrumbs.0.type" equals "navigation"
    And the event "breadcrumbs.0.metaData.url" equals "/Game/AnotherWorld"
    And the event "breadcrumbs.1.name" equals "Map Loaded"
    And the event "breadcrumbs.1.type" equals "navigation"
    And the event "breadcrumbs.1.metaData.url" equals "/Game/AnotherWorld"
    And the event "breadcrumbs.2.name" equals "Game state changed"
    And the event "breadcrumbs.2.type" equals "state"
    And the event "breadcrumbs.2.metaData.name" matches "AnotherGameState"
    And the event "metaData.unrealEngine.gameStateName" matches "AnotherGameState"
    And the event "metaData.unrealEngine.mapUrl" equals "/Game/AnotherWorld"

  Scenario: Automatic breadcrumbs for Unreal Engine user activity
    When I run "UserActivityBreadcrumbsScenario"
    And I wait to receive an error
    Then the error payload field "events.0.breadcrumbs" is an array with 1 elements
    And the event "breadcrumbs.0.name" equals "User activity changed"
    And the event "breadcrumbs.0.type" equals "user"
    And the event "breadcrumbs.0.metaData.activity" equals "Cheat mode enabled"
    And the event "metaData.unrealEngine.userActivity" equals "Cheat mode enabled"

  Scenario: Introspecting breadcrumbs at runtime
    When I run "GetCrumbScenario"
    And I wait to receive an error
    Then the error payload field "events.0.breadcrumbs" is an array with 1 elements
    And the event "breadcrumbs.0.name" equals "Introspected?"
    And the event "breadcrumbs.0.type" equals "user"
