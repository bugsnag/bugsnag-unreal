// Copyright 2021 Bugsnag. All Rights Reserved.

#include "AutomationTest.h"

#include "../WrappedStackframe.h"

#import <Bugsnag/Bugsnag.h>
#import <BugsnagPrivate/BugsnagStackframe+Private.h>

BEGIN_DEFINE_SPEC(FWrappedStackframeSpec, "Bugsnag.FWrappedStackframeSpec",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
END_DEFINE_SPEC(FWrappedStackframeSpec)
void FWrappedStackframeSpec::Define()
{
	Describe("FWrappedStackframe", [this]()
		{
			It("Gets values from the Cocoa object", [this]()
				{
					BugsnagStackframe* CocoaStackframe = [BugsnagStackframe
						stackframesWithCallStackReturnAddresses:NSThread.callStackReturnAddresses][0];

					[CocoaStackframe symbolicateIfNeeded];

					CocoaStackframe.method = @"Test::SomeFunction()";
					CocoaStackframe.type = BugsnagStackframeTypeCocoa;

					TSharedPtr<IBugsnagStackframe> Stackframe = FWrappedStackframe::From(CocoaStackframe);

					TEST_FALSE(Stackframe->GetFile().IsSet());
					TEST_FALSE(Stackframe->GetLineNumber().IsSet());
					TEST_FALSE(Stackframe->GetInProject());
					TEST_EQUAL(Stackframe->GetMethod().GetValue(), TEXT("Test::SomeFunction()"));
					TEST_EQUAL(Stackframe->GetType().GetValue(), EBugsnagErrorType::Cocoa);

					TEST_EQUAL(Stackframe->GetFrameAddress().GetValue(), (uint64)CocoaStackframe.frameAddress.unsignedLongValue);
					TEST_EQUAL(Stackframe->GetIsLR().GetValue(), CocoaStackframe.isLr);
					TEST_EQUAL(Stackframe->GetIsPC().GetValue(), CocoaStackframe.isPc);
					TEST_EQUAL(Stackframe->GetMachoFile().GetValue(), FStringFromNSString(CocoaStackframe.machoFile));
					TEST_EQUAL(Stackframe->GetMachoLoadAddress().GetValue(), (uint64)CocoaStackframe.machoLoadAddress.unsignedLongValue);
					TEST_EQUAL(Stackframe->GetMachoUuid().GetValue(), FStringFromNSString(CocoaStackframe.machoUuid));
					TEST_EQUAL(Stackframe->GetMachoVmAddress().GetValue(), (uint64)CocoaStackframe.machoVmAddress.unsignedLongValue);
					TEST_EQUAL(Stackframe->GetSymbolAddress().GetValue(), (uint64)CocoaStackframe.symbolAddress.unsignedLongValue);
				});

			It("Sets values on the Cocoa object", [this]()
				{
					BugsnagStackframe* CocoaStackframe = [[BugsnagStackframe alloc] init];

					TSharedPtr<IBugsnagStackframe> Stackframe = FWrappedStackframe::From(CocoaStackframe);

					Stackframe->SetMethod(FString(TEXT("Test::Test()")));
					TEST_EQUAL_OBJC(CocoaStackframe.method, @"Test::Test()");

					Stackframe->SetFrameAddress(0x101a);
					TEST_EQUAL((uint64)CocoaStackframe.frameAddress.unsignedLongValue, 0x101a);

					Stackframe->SetIsLR(true);
					TEST_EQUAL(CocoaStackframe.isLr, YES);

					Stackframe->SetIsPC(true);
					TEST_EQUAL(CocoaStackframe.isPc, YES);

					Stackframe->SetMachoFile(FString(TEXT("/usr/lib/libNonExistant.dylib")));
					TEST_EQUAL_OBJC(CocoaStackframe.machoFile, @"/usr/lib/libNonExistant.dylib");

					Stackframe->SetMachoLoadAddress(0x1000);
					TEST_EQUAL((uint64)CocoaStackframe.machoLoadAddress.unsignedLongValue, 0x1000);

					Stackframe->SetMachoUuid(FString(TEXT("B407B2A6-7021-4202-8E6A-8982F432C747")));
					TEST_EQUAL_OBJC(CocoaStackframe.machoUuid, @"B407B2A6-7021-4202-8E6A-8982F432C747");

					Stackframe->SetMachoVmAddress(0x0);
					TEST_EQUAL((uint64)CocoaStackframe.machoVmAddress.unsignedLongValue, 0x0);

					Stackframe->SetSymbolAddress(0x1010);
					TEST_EQUAL((uint64)CocoaStackframe.symbolAddress.unsignedLongValue, 0x1010);
				});
		});
}
