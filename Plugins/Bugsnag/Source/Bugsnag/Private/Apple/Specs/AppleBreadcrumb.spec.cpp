// Copyright 2021 Bugsnag. All Rights Reserved.

#include "AutomationTest.h"

#include "Runtime/Launch/Resources/Version.h"

#include "../AppleBreadcrumb.h"

#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION < 5)
// For UE5.0–5.4: ApplicationContextMask still exists
BEGIN_DEFINE_SPEC(FAppleBreadcrumbSpec, "Bugsnag.FAppleBreadcrumbSpec",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
#else
// For UE5.5+ (ApplicationContextMask removed)
BEGIN_DEFINE_SPEC(FAppleBreadcrumbSpec, "Bugsnag.FAppleBreadcrumbSpec",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::EditorContext)
#endif
END_DEFINE_SPEC(FAppleBreadcrumbSpec)
void FAppleBreadcrumbSpec::Define()
{
	Describe("FAppleBreadcrumb", [this]()
		{
			It("Gets values from the Cocoa object", [this]()
				{
					BugsnagBreadcrumb* CocoaBreadcrumb = [[BugsnagBreadcrumb alloc] init];
					CocoaBreadcrumb.message = @"Hello, Unreal Engine!";
					CocoaBreadcrumb.type = BSGBreadcrumbTypeLog;
					CocoaBreadcrumb.metadata = @{@"foo": @"bar"};

					TSharedPtr<IBugsnagBreadcrumb> Breadcrumb = FAppleBreadcrumb::From(CocoaBreadcrumb);
					TEST_EQUAL(Breadcrumb->GetMessage(), TEXT("Hello, Unreal Engine!"));
					TEST_EQUAL(Breadcrumb->GetType(), EBugsnagBreadcrumbType::Log);
					TEST_EQUAL(Breadcrumb->GetMetadata()->GetStringField(TEXT("foo")), TEXT("bar"));
				});

			It("Sets values on the Cocoa object", [this]()
				{
					BugsnagBreadcrumb* CocoaBreadcrumb = [[BugsnagBreadcrumb alloc] init];

					TSharedPtr<IBugsnagBreadcrumb> Breadcrumb = FAppleBreadcrumb::From(CocoaBreadcrumb);
					Breadcrumb->SetMessage(TEXT("Hello, Cocoa!"));
					Breadcrumb->SetType(EBugsnagBreadcrumbType::User);
					TEST_EQUAL_OBJC(CocoaBreadcrumb.message, @"Hello, Cocoa!");
					TEST_EQUAL(CocoaBreadcrumb.type, BSGBreadcrumbTypeUser);
				});
		});
}
