#include "Misc/AutomationTest.h"

#include "Apple/WrappedBreadcrumb.h"

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

#define TEST_EQUAL_OBJC(value, expected)                                    \
	if (![value isEqual:expected])                                          \
	{                                                                       \
		AddError(                                                           \
			FString::Printf(                                                \
				TEXT("Expected " #value " to be %s, but it was %s"),        \
				UTF8_TO_TCHAR(expected.description.UTF8String ?: "(null)"), \
				UTF8_TO_TCHAR(value.description.UTF8String ?: "(null)")),   \
			1);                                                             \
	}

BEGIN_DEFINE_SPEC(FWrappedBreadcrumbSpec, "Bugsnag.FWrappedBreadcrumbSpec",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
END_DEFINE_SPEC(FWrappedBreadcrumbSpec)
void FWrappedBreadcrumbSpec::Define()
{
	Describe("FWrappedBreadcrumb", [this]()
		{
			It("Gets values from the Cocoa object", [this]()
				{
					BugsnagBreadcrumb* CocoaBreadcrumb = [[BugsnagBreadcrumb alloc] init];
					CocoaBreadcrumb.message = @"Hello, Unreal Engine!";
					CocoaBreadcrumb.type = BSGBreadcrumbTypeLog;
					CocoaBreadcrumb.metadata = @{@"foo": @"bar"};

					TSharedPtr<IBugsnagBreadcrumb> Breadcrumb = FWrappedBreadcrumb::From(CocoaBreadcrumb);
					TEST_EQUAL(Breadcrumb->GetMessage(), TEXT("Hello, Unreal Engine!"));
					TEST_EQUAL(Breadcrumb->GetType(), EBugsnagBreadcrumbType::Log);
					TEST_EQUAL(Breadcrumb->GetMetadata()->GetStringField(TEXT("foo")), TEXT("bar"));
				});

			It("Sets values on the Cocoa object", [this]()
				{
					BugsnagBreadcrumb* CocoaBreadcrumb = [[BugsnagBreadcrumb alloc] init];

					TSharedPtr<IBugsnagBreadcrumb> Breadcrumb = FWrappedBreadcrumb::From(CocoaBreadcrumb);
					Breadcrumb->SetMessage(TEXT("Hello, Cocoa!"));
					Breadcrumb->SetType(EBugsnagBreadcrumbType::User);
					TEST_EQUAL_OBJC(CocoaBreadcrumb.message, @"Hello, Cocoa!");
					TEST_EQUAL(CocoaBreadcrumb.type, BSGBreadcrumbTypeUser);
				});
		});
}
