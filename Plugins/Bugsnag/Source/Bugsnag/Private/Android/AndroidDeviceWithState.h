#pragma once

#include "AndroidDevice.h"
#include "BugsnagDeviceWithState.h"

class FAndroidDeviceWithState : public IBugsnagDeviceWithState, FAndroidDevice
{
public:
	using FAndroidDevice::FAndroidDevice;

	TSharedPtr<uint64> GetFreeDisk() const override
	{
		return GetLongObjectField<uint64>(Cache->DeviceWithStateGetFreeDisk);
	}

	void SetFreeDisk(TSharedPtr<uint64> Value) override
	{
		SetLongObjectField<uint64>(Cache->DeviceWithStateSetFreeDisk, true, Value);
	}

	TSharedPtr<uint64> GetFreeMemory() const override
	{
		return GetLongObjectField<uint64>(Cache->DeviceWithStateGetFreeMemory);
	}

	void SetFreeMemory(TSharedPtr<uint64> Value) override
	{
		SetLongObjectField<uint64>(Cache->DeviceWithStateSetFreeMemory, true, Value);
	}

	TSharedPtr<FString> GetOrientation() const override
	{
		ReturnStringFieldPtr(JavaObject, Cache->DeviceWithStateGetOrientation);
	}

	void SetOrientation(TSharedPtr<FString> Value) override
	{
		SetStringField(Cache->DeviceWithStateSetOrientation, true, Value);
	}

	TSharedPtr<FDateTime> GetTime() const override
	{
		auto Timestamp = GetDateField(Cache->DeviceWithStateSetTime);
		return Timestamp == FDateTime(0) ? nullptr : MakeShareable(new FDateTime(Timestamp));
	}

	void SetTime(TSharedPtr<FDateTime> Value) override
	{
		SetDateField(Cache->DeviceWithStateSetTime, true, Value);
	}
};
