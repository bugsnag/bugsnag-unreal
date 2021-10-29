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

	TOptional<uint64> GetFreeDisk() const override
	{
		return OptionalUInt64FromNSNumber(CocoaDeviceWithState.freeDisk);
	}

	void SetFreeDisk(TOptional<uint64> Value) override
	{
		CocoaDeviceWithState.freeDisk = NSNumberFromOptional(Value);
	}

	// freeMemory?: number;

	TOptional<uint64> GetFreeMemory() const override
	{
		return OptionalUInt64FromNSNumber(CocoaDeviceWithState.freeMemory);
	}

	void SetFreeMemory(TOptional<uint64> Value) override
	{
		CocoaDeviceWithState.freeMemory = NSNumberFromOptional(Value);
	}

	// orientation?: string;

	TOptional<FString> GetOrientation() const override
	{
		return OptionalFromNSString(CocoaDeviceWithState.orientation);
	}

	void SetOrientation(TOptional<FString> Value) override
	{
		CocoaDeviceWithState.orientation = NSStringFromOptional(Value);
	}

	// time?: Date;

	TOptional<FDateTime> GetTime() const override
	{
		if (!CocoaDeviceWithState.time)
		{
			return TOptional<FDateTime>();
		}
		return FDateTimeFromNSDate(CocoaDeviceWithState.time);
	}

	void SetTime(TOptional<FDateTime> Value) override
	{
		CocoaDeviceWithState.time = Value.IsSet() ? NSDateFromFDateTime(Value.GetValue()) : nil;
	}

private:
	BugsnagDeviceWithState* CocoaDeviceWithState;
};
