#pragma once

#include "BugsnagDevice.h"

class BUGSNAG_API IBugsnagDeviceWithState : virtual public IBugsnagDevice
{
public:
	// freeDisk?: number;

	virtual TSharedPtr<uint64> GetFreeDisk() const = 0;

	virtual void SetFreeDisk(TSharedPtr<uint64>) = 0;

	// freeMemory?: number;

	virtual TSharedPtr<uint64> GetFreeMemory() const = 0;

	virtual void SetFreeMemory(TSharedPtr<uint64>) = 0;

	// orientation?: string;

	virtual TSharedPtr<FString> GetOrientation() const = 0;

	virtual void SetOrientation(TSharedPtr<FString>) = 0;

	// time?: Date;

	virtual TSharedPtr<FDateTime> GetTime() const = 0;

	virtual void SetTime(TSharedPtr<FDateTime>) = 0;
};
