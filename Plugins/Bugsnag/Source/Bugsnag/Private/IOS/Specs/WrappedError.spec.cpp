// Copyright 2021 Bugsnag. All Rights Reserved.

#include "AutomationTest.h"

#include "../WrappedError.h"

#import <BugsnagPrivate/BugsnagError+Private.h>
#import <BugsnagPrivate/BugsnagStackframe+Private.h>

BEGIN_DEFINE_SPEC(FWrappedErrorSpec, "Bugsnag.FWrappedErrorSpec",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
END_DEFINE_SPEC(FWrappedErrorSpec)
void FWrappedErrorSpec::Define()
{
	Describe("FWrappedError", [this]()
		{
			It("Gets values from the Cocoa object", [this]()
				{
					NSArray* ReturnAddresses = NSThread.callStackReturnAddresses;

					BugsnagError* CocoaError = [[BugsnagError alloc]
						initWithErrorClass:@"NSUnrealException"
							  errorMessage:@"Hello, Unreal Engine!"
								 errorType:BSGErrorTypeCocoa
								stacktrace:[BugsnagStackframe stackframesWithCallStackReturnAddresses:ReturnAddresses]];

					TSharedPtr<IBugsnagError> Error = FWrappedError::From(CocoaError);
					TEST_EQUAL(Error->GetErrorClass(), TEXT("NSUnrealException"));
					TEST_EQUAL(Error->GetErrorMessage(), "Hello, Unreal Engine!");
					TEST_EQUAL(Error->GetErrorType(), EBugsnagErrorType::Cocoa);
					TEST_EQUAL(Error->GetStacktrace().Num(), (int32)ReturnAddresses.count);
				});

			It("Sets values on the Cocoa object", [this]()
				{
					NSArray* ReturnAddresses = NSThread.callStackReturnAddresses;

					BugsnagError* CocoaError = [[BugsnagError alloc]
						initWithErrorClass:@""
							  errorMessage:@""
								 errorType:BSGErrorTypeCocoa
								stacktrace:[BugsnagStackframe stackframesWithCallStackReturnAddresses:ReturnAddresses]];

					TSharedPtr<IBugsnagError> Error = FWrappedError::From(CocoaError);
					Error->SetErrorClass(TEXT("Unreal Error"));
					Error->SetErrorMessage(TEXT("Hello, Cocoa!"));
					Error->SetStacktrace({});

					TEST_EQUAL_OBJC(CocoaError.errorClass, @"Unreal Error");
					TEST_EQUAL_OBJC(CocoaError.errorMessage, @"Hello, Cocoa!");
					TEST_EQUAL_OBJC(CocoaError.stacktrace, @[]);
				});
		});
}
