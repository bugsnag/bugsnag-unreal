// Copyright 2021 Bugsnag. All Rights Reserved.

#include "AutomationTest.h"

#include "Runtime/Launch/Resources/Version.h"

#include "../AppleError.h"

#import <BugsnagPrivate/BugsnagInternals.h>

#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION < 5)
// For UE5.0–5.4: ApplicationContextMask still exists
BEGIN_DEFINE_SPEC(FAppleErrorSpec, "Bugsnag.FAppleErrorSpec",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
#else
// For UE5.5+ (ApplicationContextMask removed)
BEGIN_DEFINE_SPEC(FAppleErrorSpec, "Bugsnag.FAppleErrorSpec",
    EAutomationTestFlags::ProductFilter | EAutomationTestFlags::EditorContext)
#endif
END_DEFINE_SPEC(FAppleErrorSpec)
void FAppleErrorSpec::Define()
{
	Describe("FAppleError", [this]()
		{
			It("Gets values from the Cocoa object", [this]()
				{
					NSArray* ReturnAddresses = NSThread.callStackReturnAddresses;

					BugsnagError* CocoaError = [[BugsnagError alloc]
						initWithErrorClass:@"NSUnrealException"
							  errorMessage:@"Hello, Unreal Engine!"
								 errorType:BSGErrorTypeCocoa
								stacktrace:[BugsnagStackframe stackframesWithCallStackReturnAddresses:ReturnAddresses]];

					TSharedPtr<IBugsnagError> Error = FAppleError::From(CocoaError);
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

					TSharedPtr<IBugsnagError> Error = FAppleError::From(CocoaError);
					Error->SetErrorClass(TEXT("Unreal Error"));
					Error->SetErrorMessage(TEXT("Hello, Cocoa!"));
					Error->SetStacktrace({});

					TEST_EQUAL_OBJC(CocoaError.errorClass, @"Unreal Error");
					TEST_EQUAL_OBJC(CocoaError.errorMessage, @"Hello, Cocoa!");
					TEST_EQUAL_OBJC(CocoaError.stacktrace, @[]);
				});
		});
}
