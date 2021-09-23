#pragma once

#include "BugsnagDevice.h"

class BUGSNAG_API IBugsnagDeviceWithState : public IBugsnagDevice
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

	virtual TSharedPtr<FDateTime> GetDate() const = 0;

	virtual void SetDate(TSharedPtr<FDateTime>) = 0;
};
