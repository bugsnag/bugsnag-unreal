#pragma once

#include "AppleBugsnagUtils.h"
#include "BugsnagDevice.h"

#import <Bugsnag/BugsnagDevice.h>

class FWrappedDevice : public IBugsnagDevice
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

	const TSharedPtr<FString> GetId() const
	{
		return FStringPtrFromNSString(CocoaDevice.id);
	}

	void SetId(const TSharedPtr<FString>& Value)
	{
		CocoaDevice.id = NSStringFromFStringPtr(Value);
	}

	// jailbroken?: boolean;

	TSharedPtr<bool> GetJailbroken() const
	{
		return MakeShareable(new bool(CocoaDevice.jailbroken));
	}

	void SetJailbroken(const TSharedPtr<bool>& Value)
	{
		CocoaDevice.jailbroken = Value.IsValid() && *Value;
	}

	// locale?: string;

	const TSharedPtr<FString> GetLocale() const
	{
		return FStringPtrFromNSString(CocoaDevice.locale);
	}

	void SetLocale(const TSharedPtr<FString>& Value)
	{
		CocoaDevice.locale = NSStringFromFStringPtr(Value);
	}

	// modelNumber?: string;

	const TSharedPtr<FString> GetModelNumber() const
	{
		return FStringPtrFromNSString(CocoaDevice.modelNumber);
	}

	void SetModelNumber(const TSharedPtr<FString>& Value)
	{
		CocoaDevice.modelNumber = NSStringFromFStringPtr(Value);
	}

	// manufacturer?: string;

	const TSharedPtr<FString> GetManufacturer() const
	{
		return FStringPtrFromNSString(CocoaDevice.manufacturer);
	}

	void SetManufacturer(const TSharedPtr<FString>& Value)
	{
		CocoaDevice.manufacturer = NSStringFromFStringPtr(Value);
	}

	// model?: string;

	const TSharedPtr<FString> GetModel() const
	{
		return FStringPtrFromNSString(CocoaDevice.model);
	}

	void SetModel(const TSharedPtr<FString>& Value)
	{
		CocoaDevice.model = NSStringFromFStringPtr(Value);
	}

	// osName?: string;

	const TSharedPtr<FString> GetOsName() const
	{
		return FStringPtrFromNSString(CocoaDevice.osName);
	}

	void SetOsName(const TSharedPtr<FString>& Value)
	{
		CocoaDevice.osName = NSStringFromFStringPtr(Value);
	}

	// osVersion?: string;

	const TSharedPtr<FString> GetOsVersion() const
	{
		return FStringPtrFromNSString(CocoaDevice.osVersion);
	}

	void SetOsVersion(const TSharedPtr<FString>& Value)
	{
		CocoaDevice.osVersion = NSStringFromFStringPtr(Value);
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

	const TSharedPtr<uint64> GetTotalMemory() const
	{
		return CocoaDevice.totalMemory ? MakeShareable(new uint64(CocoaDevice.totalMemory.unsignedLongValue)) : nullptr;
	}

	void SetTotalMemory(const TSharedPtr<uint64>& Value)
	{
		CocoaDevice.totalMemory = Value.IsValid() ? @(*Value) : nil;
	}

private:
	BugsnagDevice* CocoaDevice;
};
