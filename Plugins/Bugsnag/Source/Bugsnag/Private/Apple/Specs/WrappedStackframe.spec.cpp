#include "AutomationTest.h"

#include "Apple/WrappedStackframe.h"

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

					TSharedPtr<IBugsnagStackframe> Stackframe = FWrappedStackframe::From(CocoaStackframe);

					TEST_TRUE(Stackframe->GetFile() == nullptr);
					TEST_TRUE(Stackframe->GetLineNumber() == nullptr);
					TEST_FALSE(Stackframe->GetInProject());
					TEST_EQUAL(*(Stackframe->GetMethod()), TEXT("Test::SomeFunction()"));
					TEST_EQUAL(*(Stackframe->GetType()), EBugsnagErrorType::Cocoa);

					TEST_EQUAL(*(Stackframe->GetFrameAddress()), (uint64)CocoaStackframe.frameAddress.unsignedLongValue);
					TEST_EQUAL(*(Stackframe->GetIsLR()), CocoaStackframe.isLr);
					TEST_EQUAL(*(Stackframe->GetIsPC()), CocoaStackframe.isPc);
					TEST_EQUAL(*(Stackframe->GetMachoFile()), FStringFromNSString(CocoaStackframe.machoFile));
					TEST_EQUAL(*(Stackframe->GetMachoLoadAddress()), (uint64)CocoaStackframe.machoLoadAddress.unsignedLongValue);
					TEST_EQUAL(*(Stackframe->GetMachoUuid()), FStringFromNSString(CocoaStackframe.machoUuid));
					TEST_EQUAL(*(Stackframe->GetMachoVmAddress()), (uint64)CocoaStackframe.machoVmAddress.unsignedLongValue);
					TEST_EQUAL(*(Stackframe->GetSymbolAddress()), (uint64)CocoaStackframe.symbolAddress.unsignedLongValue);
				});

			It("Sets values on the Cocoa object", [this]()
				{
					BugsnagStackframe* CocoaStackframe = [[BugsnagStackframe alloc] init];

					TSharedPtr<IBugsnagStackframe> Stackframe = FWrappedStackframe::From(CocoaStackframe);

					Stackframe->SetMethod(MakeShareable(new FString(TEXT("Test::Test()"))));
					TEST_EQUAL_OBJC(CocoaStackframe.method, @"Test::Test()");

					Stackframe->SetType(MakeShareable(new EBugsnagErrorType(EBugsnagErrorType::C)));
					TEST_EQUAL_OBJC(CocoaStackframe.type, @"c");

					Stackframe->SetFrameAddress(MakeShareable(new uint64(0x101a)));
					TEST_EQUAL((uint64)CocoaStackframe.frameAddress.unsignedLongValue, 0x101a);

					Stackframe->SetIsLR(MakeShareable(new bool(true)));
					TEST_EQUAL(CocoaStackframe.isLr, YES);

					Stackframe->SetIsPC(MakeShareable(new bool(true)));
					TEST_EQUAL(CocoaStackframe.isPc, YES);

					Stackframe->SetMachoFile(MakeShareable(new FString(TEXT("/usr/lib/libNonExistant.dylib"))));
					TEST_EQUAL_OBJC(CocoaStackframe.machoFile, @"/usr/lib/libNonExistant.dylib");

					Stackframe->SetMachoLoadAddress(MakeShareable(new uint64(0x1000)));
					TEST_EQUAL((uint64)CocoaStackframe.machoLoadAddress.unsignedLongValue, 0x1000);

					Stackframe->SetMachoUuid(MakeShareable(new FString(TEXT("B407B2A6-7021-4202-8E6A-8982F432C747"))));
					TEST_EQUAL_OBJC(CocoaStackframe.machoUuid, @"B407B2A6-7021-4202-8E6A-8982F432C747");

					Stackframe->SetMachoVmAddress(MakeShareable(new uint64(0x0)));
					TEST_EQUAL((uint64)CocoaStackframe.machoVmAddress.unsignedLongValue, 0x0);

					Stackframe->SetSymbolAddress(MakeShareable(new uint64(0x1010)));
					TEST_EQUAL((uint64)CocoaStackframe.symbolAddress.unsignedLongValue, 0x1010);
				});
		});
}
