#include "Misc/AutomationTest.h"

//
// Declare the Automation Test.
//
// Each test case is implemented as a subclass of FAutomationTestBase.
//
// Test flags determine, among other things, which contexts the tests can be run in, e.g.
// - EditorContext	Test is suitable for running within the editor
// - ClientContext	Test is suitable for running within the client (game)
//
// https://docs.unrealengine.com/en-US/TestingAndOptimization/Automation/TechnicalGuide/
//
// To run tests from Unreal Editor, open Windows -> Developer Tools -> Session Frontend and then select the Automation sub-tab.
// To make a connected device appear in the "My Sessions" list, launch the example app on it first (the example app acts as the test runner.)
//
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExampleTest, "Bugsnag.BugsnagTests.ExampleTest", EAutomationTestFlags::ClientContext | EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter)

//
// All Automation Test classes must implement RunTest.
//
bool FExampleTest::RunTest(const FString& Parameters)
{
	//
	// FAutomationTestBase implements many methods of the form TextXzy() which can be used to assert test expectations.
	//
	// https://docs.unrealengine.com/en-US/API/Runtime/Core/Misc/FAutomationTestBase/
	//
	TestTrueExpr(true);

	return true;
}
