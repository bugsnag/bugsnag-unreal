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

  Scenario: Introspecting breadcrumbs at runtime
    When I run "GetCrumbScenario"
    And I wait to receive an error
    Then the error payload field "events.0.breadcrumbs" is an array with 1 elements
    And the event "breadcrumbs.0.name" equals "Introspected?"
    And the event "breadcrumbs.0.type" equals "user"
