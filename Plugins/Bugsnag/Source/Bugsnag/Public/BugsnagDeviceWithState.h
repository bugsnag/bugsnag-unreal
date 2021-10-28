#pragma once

#include "BugsnagDevice.h"

class BUGSNAG_API IBugsnagDeviceWithState : virtual public IBugsnagDevice
{
public:
	// freeDisk?: number;

	virtual TOptional<uint64> GetFreeDisk() const = 0;

	virtual void SetFreeDisk(TOptional<uint64>) = 0;

	// freeMemory?: number;

	virtual TOptional<uint64> GetFreeMemory() const = 0;

	virtual void SetFreeMemory(TOptional<uint64>) = 0;

	// orientation?: string;

	virtual TSharedPtr<FString> GetOrientation() const = 0;

	virtual void SetOrientation(TSharedPtr<FString>) = 0;

	// time?: Date;

	virtual TOptional<FDateTime> GetTime() const = 0;

	virtual void SetTime(TOptional<FDateTime>) = 0;
};
