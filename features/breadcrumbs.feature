Feature: Breadcrumbs and modifying with callbacks

  Scenario: Filtering and modifying breadcrumb contents
    When I run "ModifyCrumbsScenario"
    And I wait to receive an error
    Then the error is valid for the error reporting API version "4.0" for the "Unreal Bugsnag Notifier" notifier
    And the error payload field "events.0.breadcrumbs" is an array with 1 elements
    And the error payload field "events.0.breadcrumbs.0.name" equals "Crash time now"
    And the error payload field "events.0.breadcrumbs.0.type" equals "request"
    And the error payload field "events.0.breadcrumbs.0.metaData.forty" equals "four zero"
    And the error payload field "events.0.breadcrumbs.0.metaData.macaron" equals 3
    And the error payload field "events.0.breadcrumbs.0.metaData.cronut" is false
