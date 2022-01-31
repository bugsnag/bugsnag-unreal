// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "AppleBugsnagUtils.h"
#include "BugsnagDevice.h"

#import <Bugsnag/BugsnagDevice.h>

class FWrappedDevice : virtual public IBugsnagDevice
{
public:
	static TSharedRef<FWrappedDevice> From(BugsnagDevice* CocoaDevice)
	{
		return MakeShared<FWrappedDevice>(CocoaDevice);
	}

	FWrappedDevice(BugsnagDevice* CocoaDevice)
		: CocoaDevice(CocoaDevice)
	{
	}

	// cpuAbi?: string[];

	const TSharedPtr<TArray<FString>> GetCpuAbi() const
	{
		// Not supported by bugsnag-cocoa
		return nullptr;
	}

	void SetCpuAbi(const TSharedPtr<TArray<FString>>&)
	{
		// Not supported by bugsnag-cocoa
	}

	// id?: string;

	const TOptional<FString> GetId() const
	{
		return OptionalFromNSString(CocoaDevice.id);
	}

	void SetId(const TOptional<FString>& Value)
	{
		CocoaDevice.id = NSStringFromOptional(Value);
	}

	// jailbroken?: boolean;

	TOptional<bool> GetJailbroken() const
	{
		return CocoaDevice.jailbroken;
	}

	void SetJailbroken(const TOptional<bool>& Value)
	{
		CocoaDevice.jailbroken = Value.IsSet() && Value.GetValue();
	}

	// locale?: string;

	const TOptional<FString> GetLocale() const
	{
		return OptionalFromNSString(CocoaDevice.locale);
	}

	void SetLocale(const TOptional<FString>& Value)
	{
		CocoaDevice.locale = NSStringFromOptional(Value);
	}

	// modelNumber?: string;

	const TOptional<FString> GetModelNumber() const
	{
		return OptionalFromNSString(CocoaDevice.modelNumber);
	}

	void SetModelNumber(const TOptional<FString>& Value)
	{
		CocoaDevice.modelNumber = NSStringFromOptional(Value);
	}

	// manufacturer?: string;

	const TOptional<FString> GetManufacturer() const
	{
		return OptionalFromNSString(CocoaDevice.manufacturer);
	}

	void SetManufacturer(const TOptional<FString>& Value)
	{
		CocoaDevice.manufacturer = NSStringFromOptional(Value);
	}

	// model?: string;

	const TOptional<FString> GetModel() const
	{
		return OptionalFromNSString(CocoaDevice.model);
	}

	void SetModel(const TOptional<FString>& Value)
	{
		CocoaDevice.model = NSStringFromOptional(Value);
	}

	// osName?: string;

	const TOptional<FString> GetOsName() const
	{
		return OptionalFromNSString(CocoaDevice.osName);
	}

	void SetOsName(const TOptional<FString>& Value)
	{
		CocoaDevice.osName = NSStringFromOptional(Value);
	}

	// osVersion?: string;

	const TOptional<FString> GetOsVersion() const
	{
		return OptionalFromNSString(CocoaDevice.osVersion);
	}

	void SetOsVersion(const TOptional<FString>& Value)
	{
		CocoaDevice.osVersion = NSStringFromOptional(Value);
	}

	// runtimeVersions?: map[string->string];

	const TSharedPtr<TMap<FString, FString>> GetRuntimeVersions() const
	{
		if (!CocoaDevice.runtimeVersions)
		{
			return nullptr;
		}
		TSharedPtr<TMap<FString, FString>> RuntimeVersions = MakeShareable(new TMap<FString, FString>);
		for (NSString* Key in CocoaDevice.runtimeVersions)
		{
			id Value = CocoaDevice.runtimeVersions[Key];
			if ([Value isKindOfClass:[NSString class]])
			{
				RuntimeVersions->Add(FStringFromNSString(Key), FStringFromNSString(Value));
			}
		}
		return RuntimeVersions;
	}

	void SetRuntimeVersions(const TSharedPtr<TMap<FString, FString>>& Value)
	{
		if (!Value.IsValid())
		{
			CocoaDevice.runtimeVersions = nil;
			return;
		}
		NSMutableDictionary* Dictionary = [NSMutableDictionary dictionary];
		for (const TPair<FString, FString>& Pair : *Value)
		{
			Dictionary[NSStringFromFString(Pair.Key)] = NSStringFromFString(Pair.Value);
		}
		CocoaDevice.runtimeVersions = Dictionary;
	}

	// totalMemory?: number;

	const TOptional<uint64> GetTotalMemory() const
	{
		return OptionalUInt64FromNSNumber(CocoaDevice.totalMemory);
	}

	void SetTotalMemory(const TOptional<uint64>& Value)
	{
		CocoaDevice.totalMemory = NSNumberFromOptional(Value);
	}

private:
	BugsnagDevice* CocoaDevice;
};
