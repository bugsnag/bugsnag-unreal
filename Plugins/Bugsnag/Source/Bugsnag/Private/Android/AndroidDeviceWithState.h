#pragma once

#include "AndroidDevice.h"
#include "BugsnagDeviceWithState.h"

class FAndroidDeviceWithState : public IBugsnagDeviceWithState, FAndroidDevice
{
public:
	using FAndroidDevice::FAndroidDevice;

	TOptional<uint64> GetFreeDisk() const override
	{
		return GetLongObjectField<uint64>(Cache->DeviceWithStateGetFreeDisk);
	}

	void SetFreeDisk(TOptional<uint64> Value) override
	{
		SetLongObjectField<uint64>(Cache->DeviceWithStateSetFreeDisk, true, Value);
	}

	TOptional<uint64> GetFreeMemory() const override
	{
		return GetLongObjectField<uint64>(Cache->DeviceWithStateGetFreeMemory);
	}

	void SetFreeMemory(TOptional<uint64> Value) override
	{
		SetLongObjectField<uint64>(Cache->DeviceWithStateSetFreeMemory, true, Value);
	}

	TOptional<FString> GetOrientation() const override
	{
		ReturnStringFieldOptional(JavaObject, Cache->DeviceWithStateGetOrientation);
	}

	void SetOrientation(TOptional<FString> Value) override
	{
		SetStringField(Cache->DeviceWithStateSetOrientation, true, Value);
	}

	TOptional<FDateTime> GetTime() const override
	{
		auto Timestamp = GetDateField(Cache->DeviceWithStateGetTime);
		return Timestamp == FDateTime(0) ? TOptional<FDateTime>() : Timestamp;
	}

	void SetTime(TOptional<FDateTime> Value) override
	{
		SetDateField(Cache->DeviceWithStateSetTime, true, Value);
	}
};
