#include "AutomationTest.h"

#include "../WrappedBreadcrumb.h"

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
