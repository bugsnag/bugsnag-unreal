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

  Scenario: Automatic breadcrumbs for Unreal Engine map changes
    When I run "OpenLevelBreadcrumbsScenario"
    And I wait to receive an error
    And the event "breadcrumbs.0.name" equals "Map Loading"
    And the event "breadcrumbs.0.type" equals "navigation"
    And the event "breadcrumbs.0.metaData.url" equals "/Game/AnotherWorld"
    And the event "breadcrumbs.1.name" equals "Map Loaded"
    And the event "breadcrumbs.1.type" equals "navigation"
    And the event "breadcrumbs.1.metaData.url" equals "/Game/AnotherWorld"
