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

	TSharedPtr<int32> GetLineNumber() const override
	{
		// Not supported by bugsnag-cocoa
		return nullptr;
	}

	void SetLineNumber(const TSharedPtr<int32>&) override
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

	TSharedPtr<EBugsnagErrorType> GetType() const override
	{
		if (CocoaStackframe.type)
		{
			if ([CocoaStackframe.type isEqual:BugsnagStackframeTypeCocoa])
			{
				return MakeShareable(new EBugsnagErrorType(EBugsnagErrorType::Cocoa));
			}
			else
			{
				// Note: this is inaccurate
				return MakeShareable(new EBugsnagErrorType(EBugsnagErrorType::C));
			}
		}
		else
		{
			return nullptr;
		}
	}

	// machoVMAddress?: number

	TSharedPtr<uint64> GetMachoVmAddress() const override
	{
		return UInt64PtrFromNSNumber(CocoaStackframe.machoVmAddress);
	}

	void SetMachoVmAddress(const TSharedPtr<uint64>& Value) override
	{
		CocoaStackframe.machoVmAddress = Value.IsValid() ? @(*Value) : nil;
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

	TSharedPtr<uint64> GetSymbolAddress() const override
	{
		return UInt64PtrFromNSNumber(CocoaStackframe.symbolAddress);
	}

	void SetSymbolAddress(const TSharedPtr<uint64>& Value) override
	{
		CocoaStackframe.symbolAddress = Value.IsValid() ? @(*Value) : nil;
	}

	// machoLoadAddress?: number

	TSharedPtr<uint64> GetMachoLoadAddress() const override
	{
		return UInt64PtrFromNSNumber(CocoaStackframe.machoLoadAddress);
	}

	void SetMachoLoadAddress(const TSharedPtr<uint64>& Value) override
	{
		CocoaStackframe.machoLoadAddress = Value.IsValid() ? @(*Value) : nil;
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

	TSharedPtr<uint64> GetFrameAddress() const override
	{
		return UInt64PtrFromNSNumber(CocoaStackframe.frameAddress);
	}

	void SetFrameAddress(const TSharedPtr<uint64>& Value) override
	{
		CocoaStackframe.frameAddress = Value.IsValid() ? @(*Value) : nil;
	}

	// isPC?: boolean

	TSharedPtr<bool> GetIsPC() const override
	{
		return MakeShareable(new bool(CocoaStackframe.isPc));
	}

	void SetIsPC(const TSharedPtr<bool>& Value) override
	{
		CocoaStackframe.isPc = Value.IsValid() && *Value;
	}

	// isLR?: boolean

	TSharedPtr<bool> GetIsLR() const override
	{
		return MakeShareable(new bool(CocoaStackframe.isLr));
	}

	void SetIsLR(const TSharedPtr<bool>& Value) override
	{
		CocoaStackframe.isLr = Value.IsValid() && *Value;
	}

private:
	BugsnagStackframe* CocoaStackframe;

	friend class FWrappedError;
	friend class FWrappedThread;
};
