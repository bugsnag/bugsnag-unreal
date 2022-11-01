// Copyright 2021 Bugsnag. All Rights Reserved.

#include "AutomationTest.h"

#include "../AppleThread.h"

#import <BugsnagPrivate/BugsnagInternals.h>

BEGIN_DEFINE_SPEC(FAppleThreadSpec, "Bugsnag.FAppleThreadSpec",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
END_DEFINE_SPEC(FAppleThreadSpec)
void FAppleThreadSpec::Define()
{
	Describe("FAppleThread", [this]()
		{
			It("Gets values from the Cocoa object", [this]()
				{
					NSArray* ReturnAddresses = NSThread.callStackReturnAddresses;

					BugsnagThread* CocoaThread = [BugsnagThread allThreads:NO callStackReturnAddresses:ReturnAddresses][0];

					TSharedPtr<IBugsnagThread> Thread = FAppleThread::From(CocoaThread);

					CocoaThread.id = @"123";
					TEST_EQUAL(Thread->GetId(), TEXT("123"));

					CocoaThread.name = @"Test thread";
					TEST_EQUAL(Thread->GetName(), TEXT("Test thread"));

					TEST_TRUE(Thread->GetErrorReportingThread());
					TEST_EQUAL(Thread->GetErrorType(), EBugsnagErrorType::Cocoa);
					TEST_EQUAL(Thread->GetStacktrace().Num(), (int)ReturnAddresses.count);

					CocoaThread.stacktrace = @[];
					TEST_EQUAL(Thread->GetStacktrace().Num(), 0);
				});

			It("Sets values on the Cocoa object", [this]()
				{
					NSArray* ReturnAddresses = NSThread.callStackReturnAddresses;

					BugsnagThread* CocoaThread = [BugsnagThread allThreads:NO callStackReturnAddresses:ReturnAddresses][0];

					TSharedPtr<IBugsnagThread> Thread = FAppleThread::From(CocoaThread);

					Thread->SetId(TEXT("234"));
					TEST_EQUAL_OBJC(CocoaThread.id, @"234");

					Thread->SetName(TEXT("Awesome thread"));
					TEST_EQUAL_OBJC(CocoaThread.name, @"Awesome thread");

					auto Stacktrace = Thread->GetStacktrace();
					Stacktrace.RemoveAt(4, Stacktrace.Num() - 4);
					Thread->SetStacktrace(Stacktrace);
					TEST_EQUAL((int)CocoaThread.stacktrace.count, 4);
				});
		});
}
