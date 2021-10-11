#pragma once

#include "CoreMinimal.h"

#include "AppleBugsnagUtils.h"
#include "BugsnagDeviceWithState.h"
#include "WrappedDevice.h"

#import <Bugsnag/BugsnagDeviceWithState.h>

class BUGSNAG_API FWrappedDeviceWithState : public IBugsnagDeviceWithState, FWrappedDevice
{
public:
	static TSharedRef<FWrappedDeviceWithState> From(BugsnagDeviceWithState* CocoaDeviceWithState)
	{
		return MakeShared<FWrappedDeviceWithState>(CocoaDeviceWithState);
	}

	FWrappedDeviceWithState(BugsnagDeviceWithState* CocoaDeviceWithState)
		: FWrappedDevice(CocoaDeviceWithState)
		, CocoaDeviceWithState(CocoaDeviceWithState)
	{
	}

	// freeDisk?: number;

	TSharedPtr<uint64> GetFreeDisk() const override
	{
		return UInt64PtrFromNSNumber(CocoaDeviceWithState.freeDisk);
	}

	void SetFreeDisk(TSharedPtr<uint64> Value) override
	{
		CocoaDeviceWithState.freeDisk = Value.IsValid() ? @(*Value) : nil;
	}

	// freeMemory?: number;

	TSharedPtr<uint64> GetFreeMemory() const override
	{
		return UInt64PtrFromNSNumber(CocoaDeviceWithState.freeMemory);
	}

	void SetFreeMemory(TSharedPtr<uint64> Value) override
	{
		CocoaDeviceWithState.freeMemory = Value.IsValid() ? @(*Value) : nil;
	}

	// orientation?: string;

	TSharedPtr<FString> GetOrientation() const override
	{
		return FStringPtrFromNSString(CocoaDeviceWithState.orientation);
	}

	void SetOrientation(TSharedPtr<FString> Value) override
	{
		CocoaDeviceWithState.orientation = NSStringFromFStringPtr(Value);
	}

	// time?: Date;

	TSharedPtr<FDateTime> GetTime() const override
	{
		if (!CocoaDeviceWithState.time)
		{
			return nullptr;
		}
		FDateTime DateTime = FDateTimeFromNSDate(CocoaDeviceWithState.time);
		return MakeShareable(new FDateTime(DateTime));
	}

	void SetTime(TSharedPtr<FDateTime> Value) override
	{
		CocoaDeviceWithState.time = Value.IsValid() ? NSDateFromFDateTime(*Value) : nil;
	}

private:
	BugsnagDeviceWithState* CocoaDeviceWithState;
};
