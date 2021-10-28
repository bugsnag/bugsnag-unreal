#pragma once

#include "CoreMinimal.h"

#include "AppleBugsnagUtils.h"
#include "BugsnagStackframe.h"

#import <Bugsnag/BugsnagStackframe.h>

class BUGSNAG_API FWrappedStackframe : public IBugsnagStackframe
{
public:
	static TSharedRef<FWrappedStackframe> From(BugsnagStackframe* CocoaStackframe)
	{
		return MakeShared<FWrappedStackframe>(CocoaStackframe);
	}

	FWrappedStackframe(BugsnagStackframe* CocoaStackframe)
		: CocoaStackframe(CocoaStackframe)
	{
	}

	// file?: string,

	TSharedPtr<FString> GetFile() const override
	{
		// Not supported by bugsnag-cocoa
		return nullptr;
	}

	void SetFile(const TSharedPtr<FString>& Value) override
	{
		// Not supported by bugsnag-cocoa
	}

	// inProject: boolean,

	bool GetInProject() const override
	{
		// Not supported by bugsnag-cocoa
		return false;
	}

	void SetInProject(bool) override
	{
		// Not supported by bugsnag-cocoa
	}

	// lineNumber?: number,

	TOptional<int32> GetLineNumber() const override
	{
		// Not supported by bugsnag-cocoa
		return TOptional<int32>();
	}

	void SetLineNumber(const TOptional<int32>&) override
	{
		// Not supported by bugsnag-cocoa
	}

	// method?: string,

	TSharedPtr<FString> GetMethod() const override
	{
		return FStringPtrFromNSString(CocoaStackframe.method);
	}

	void SetMethod(const TSharedPtr<FString>& Value) override
	{
		CocoaStackframe.method = NSStringFromFStringPtr(Value);
	}

	// type?: ErrorType,

	TOptional<EBugsnagErrorType> GetType() const override
	{
		if (CocoaStackframe.type)
		{
			if ([CocoaStackframe.type isEqual:BugsnagStackframeTypeCocoa])
			{
				return EBugsnagErrorType::Cocoa;
			}
			else
			{
				// Note: this is inaccurate
				return EBugsnagErrorType::C;
			}
		}
		else
		{
			return TOptional<EBugsnagErrorType>();
		}
	}

	// machoVMAddress?: number

	TOptional<uint64> GetMachoVmAddress() const override
	{
		return OptionalUInt64FromNSNumber(CocoaStackframe.machoVmAddress);
	}

	void SetMachoVmAddress(const TOptional<uint64>& Value) override
	{
		CocoaStackframe.machoVmAddress = NSNumberFromOptional(Value);
	}

	// machoFile?: string

	TSharedPtr<FString> GetMachoFile() const override
	{
		return FStringPtrFromNSString(CocoaStackframe.machoFile);
	}

	void SetMachoFile(const TSharedPtr<FString>& Value) override
	{
		CocoaStackframe.machoFile = NSStringFromFStringPtr(Value);
	}

	// symbolAddress?: number

	TOptional<uint64> GetSymbolAddress() const override
	{
		return OptionalUInt64FromNSNumber(CocoaStackframe.symbolAddress);
	}

	void SetSymbolAddress(const TOptional<uint64>& Value) override
	{
		CocoaStackframe.symbolAddress = NSNumberFromOptional(Value);
	}

	// machoLoadAddress?: number

	TOptional<uint64> GetMachoLoadAddress() const override
	{
		return OptionalUInt64FromNSNumber(CocoaStackframe.machoLoadAddress);
	}

	void SetMachoLoadAddress(const TOptional<uint64>& Value) override
	{
		CocoaStackframe.machoLoadAddress = NSNumberFromOptional(Value);
	}

	// machoUUID?: string

	TSharedPtr<FString> GetMachoUuid() const override
	{
		return FStringPtrFromNSString(CocoaStackframe.machoUuid);
	}

	void SetMachoUuid(const TSharedPtr<FString>& Value) override
	{
		CocoaStackframe.machoUuid = NSStringFromFStringPtr(Value);
	}

	// frameAddress?: number

	TOptional<uint64> GetFrameAddress() const override
	{
		return OptionalUInt64FromNSNumber(CocoaStackframe.frameAddress);
	}

	void SetFrameAddress(const TOptional<uint64>& Value) override
	{
		CocoaStackframe.frameAddress = NSNumberFromOptional(Value);
	}

	// isPC?: boolean

	TOptional<bool> GetIsPC() const override
	{
		return CocoaStackframe.isPc;
	}

	void SetIsPC(const TOptional<bool>& Value) override
	{
		CocoaStackframe.isPc = Value.Get(false);
	}

	// isLR?: boolean

	TOptional<bool> GetIsLR() const override
	{
		return CocoaStackframe.isLr;
	}

	void SetIsLR(const TOptional<bool>& Value) override
	{
		CocoaStackframe.isLr = Value.Get(false);
	}

private:
	BugsnagStackframe* CocoaStackframe;

	friend class FWrappedError;
	friend class FWrappedThread;
};
