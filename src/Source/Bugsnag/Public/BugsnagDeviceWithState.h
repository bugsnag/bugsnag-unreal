// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "BugsnagDevice.h"

/**
 * Stateful information about the computer or device running the app.
 *
 * Can be amended from an OnError callback if necessary.
 */
class BUGSNAG_API IBugsnagDeviceWithState : virtual public IBugsnagDevice
{
public:
	/**
	 * The number of unused bytes of storage available on the device.
	 */
	virtual TOptional<uint64> GetFreeDisk() const = 0;

	virtual void SetFreeDisk(TOptional<uint64>) = 0;

	/**
	 * The available RAM on the device (in bytes.)
	 */
	virtual TOptional<uint64> GetFreeMemory() const = 0;

	virtual void SetFreeMemory(TOptional<uint64>) = 0;

	/**
	 * The orientation of the device when the event occurred.
	 */
	virtual TOptional<FString> GetOrientation() const = 0;

	virtual void SetOrientation(TOptional<FString>) = 0;

	/**
	 * The timestamp on the device when the event occurred.
	 */
	virtual TOptional<FDateTime> GetTime() const = 0;

	virtual void SetTime(TOptional<FDateTime>) = 0;
};
