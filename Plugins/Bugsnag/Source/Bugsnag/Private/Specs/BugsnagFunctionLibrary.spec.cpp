#include "Misc/AutomationTest.h"

#include "BugsnagFunctionLibrary.h"

#define TEST_TRUE(expression) \
	TestTrue(TEXT(#expression), expression)

#define TEST_FALSE(expression) \
	TestFalse(TEXT(#expression), expression)

#define TEST_EQUAL(expression, expected) \
	TestEqual(TEXT(#expression), expression, expected)

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
