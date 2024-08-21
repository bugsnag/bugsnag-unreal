// Copyright 2021 Bugsnag. All Rights Reserved.

#include "AutomationTest.h"

#include "../AppleEvent.h"

#import <BugsnagPrivate/BugsnagInternals.h>

BEGIN_DEFINE_SPEC(FAppleEventSpec, "Bugsnag.FAppleEventSpec",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
END_DEFINE_SPEC(FAppleEventSpec)
void FAppleEventSpec::Define()
{
	Describe("FAppleEvent", [this]()
		{
			It("Gets values from the Cocoa object", [this]()
				{
					BugsnagHandledState* HandledState = [BugsnagHandledState
						handledStateWithSeverityReason:HandledError
											  severity:BSGSeverityWarning
											 attrValue:nil];

					BugsnagEvent* CocoaEvent = [[BugsnagEvent alloc]
						 initWithApp:[[BugsnagAppWithState alloc] init]
							  device:[[BugsnagDeviceWithState alloc] init]
						handledState:HandledState
								user:[[BugsnagUser alloc] init]
							metadata:[[BugsnagMetadata alloc] init]
						 breadcrumbs:@[]
							  errors:@[]
							 threads:@[]
							 session:nil];

					TSharedPtr<IBugsnagEvent> Event = FAppleEvent::From(CocoaEvent);
					TEST_FALSE(Event->GetApiKey().IsSet());
					TEST_FALSE(Event->GetContext().IsSet());
					TEST_FALSE(Event->GetGroupingHash().IsSet());
					TEST_EQUAL(Event->GetBreadcrumbs().Num(), 0);
					TEST_EQUAL(Event->GetErrors().Num(), 0);
					TEST_EQUAL(Event->GetThreads().Num(), 0);
					TEST_EQUAL(Event->GetSeverity(), EBugsnagSeverity::Warning);
					TEST_EQUAL(Event->GetUnhandled(), false);

					CocoaEvent.apiKey = @"0192837465afbecd0192837465afbecd";
					TEST_EQUAL(Event->GetApiKey().GetValue(), TEXT("0192837465afbecd0192837465afbecd"));

					CocoaEvent.context = @"Level three";
					TEST_EQUAL(Event->GetContext().GetValue(), TEXT("Level three"));

					CocoaEvent.groupingHash = @"UnrealEngine.cpp:32";
					TEST_EQUAL(Event->GetGroupingHash().GetValue(), TEXT("UnrealEngine.cpp:32"));

					CocoaEvent.breadcrumbs = @[[[BugsnagBreadcrumb alloc] init]];
					TEST_EQUAL(Event->GetBreadcrumbs().Num(), 1);

					CocoaEvent.errors = @[[[BugsnagError alloc] init]];
					TEST_EQUAL(Event->GetErrors().Num(), 1);

					CocoaEvent.threads = @[[[BugsnagThread alloc] init]];
					TEST_EQUAL(Event->GetThreads().Num(), 1);

					CocoaEvent.severity = BSGSeverityInfo;
					TEST_EQUAL(Event->GetSeverity(), EBugsnagSeverity::Info);

					CocoaEvent.unhandled = true;
					TEST_EQUAL(Event->GetUnhandled(), true);
				});

			It("Sets values on the Cocoa object", [this]()
				{
					BugsnagHandledState* HandledState = [BugsnagHandledState
						handledStateWithSeverityReason:HandledError
											  severity:BSGSeverityWarning
											 attrValue:nil];

					BugsnagEvent* CocoaEvent = [[BugsnagEvent alloc]
						 initWithApp:[[BugsnagAppWithState alloc] init]
							  device:[[BugsnagDeviceWithState alloc] init]
						handledState:HandledState
								user:[[BugsnagUser alloc] init]
							metadata:[[BugsnagMetadata alloc] init]
						 breadcrumbs:@[]
							  errors:@[]
							 threads:@[]
							 session:nil];

					TSharedPtr<IBugsnagEvent> Event = FAppleEvent::From(CocoaEvent);

					Event->SetApiKey(FString(TEXT("0192837465afbecd0192837465afbecd")));
					TEST_EQUAL_OBJC(CocoaEvent.apiKey, @"0192837465afbecd0192837465afbecd");

					Event->SetContext(FString(TEXT("Some Context")));
					TEST_EQUAL_OBJC(CocoaEvent.context, @"Some Context");

					Event->SetGroupingHash(FString(TEXT("Group On This")));
					TEST_EQUAL_OBJC(CocoaEvent.groupingHash, @"Group On This");

					Event->SetSeverity(EBugsnagSeverity::Error);
					TEST_EQUAL(CocoaEvent.severity, BSGSeverityError);

					Event->SetUnhandled(true);
					TEST_EQUAL(CocoaEvent.unhandled, true);
				});
		});
}
