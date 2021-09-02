#include "Misc/AutomationTest.h"

#include "BugsnagFunctionLibrary.h"

//
// Macros copied from Engine/Source/Developer/AutomationDriver/Private/Specs/AutomationDriver.spec.cpp
//
#define TEST_TRUE(expression) \
	EPIC_TEST_BOOLEAN_(TEXT(#expression), expression, true)

#define TEST_FALSE(expression) \
	EPIC_TEST_BOOLEAN_(TEXT(#expression), expression, false)

#define TEST_EQUAL(expression, expected) \
	EPIC_TEST_BOOLEAN_(TEXT(#expression), expression, expected)

#define EPIC_TEST_BOOLEAN_(text, expression, expected) \
	TestEqual(text, expression, expected);

//
// This is an example of an Automation Spec, a newer type of test that follows BDD methodology.
//
// https://docs.unrealengine.com/en-US/TestingAndOptimization/Automation/AutomationSpec/
//
BEGIN_DEFINE_SPEC(UBugsnagFunctionLibrarySpec, "Bugsnag.UBugsnagFunctionLibrarySpec",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
END_DEFINE_SPEC(UBugsnagFunctionLibrarySpec)
void UBugsnagFunctionLibrarySpec::Define()
{
	Describe("CaptureStackTrace", [this]()
		{
			It("Should return sensible values", [this]()
				{
					TArray<uint64> StackTrace = UBugsnagFunctionLibrary::CaptureStackTrace();
					TEST_TRUE(StackTrace.Num() > 0);

					for (uint64 Address : StackTrace)
					{
						TEST_TRUE(Address != 0);
					}
				});
		});
}
