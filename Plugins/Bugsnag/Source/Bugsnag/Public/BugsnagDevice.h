#pragma once

class BUGSNAG_API IBugsnagDevice
{
public:
	// cpuAbi?: string[];

	virtual const TSharedPtr<TArray<FString>> GetCpuAbi() const = 0;

	virtual void SetCpuAbi(const TSharedPtr<TArray<FString>>&) = 0;

	// id?: string;

	virtual const TSharedPtr<FString> GetId() const = 0;

	virtual void SetId(const TSharedPtr<FString>&) = 0;

	// jailbroken?: boolean;

	virtual TSharedPtr<bool> GetJailbroken() const = 0;

	virtual void SetJailbroken(const TSharedPtr<bool>&) = 0;

	// locale?: string;

	virtual const TSharedPtr<FString> GetLocale() const = 0;

	virtual void SetLocale(const TSharedPtr<FString>&) = 0;

	// modelNumber?: string;

	virtual const TSharedPtr<FString> GetModelNumber() const = 0;

	virtual void SetModelNumber(const TSharedPtr<FString>&) = 0;

	// manufacturer?: string;

	virtual const TSharedPtr<FString> GetManufacturer() const = 0;

	virtual void SetManufacturer(const TSharedPtr<FString>&) = 0;

	// model?: string;

	virtual const TSharedPtr<FString> GetModel() const = 0;

	virtual void SetModel(const TSharedPtr<FString>&) = 0;

	// osName?: string;

	virtual const TSharedPtr<FString> GetOsName() const = 0;

	virtual void SetOsName(const TSharedPtr<FString>&) = 0;

	// osVersion?: string;

	virtual const TSharedPtr<FString> GetOsVersion() const = 0;

	virtual void SetOsVersion(const TSharedPtr<FString>&) = 0;

	// runtimeVersions?: map[string->string];

	virtual const TSharedPtr<TMap<FString, FString>> GetRuntimeVersions() const = 0;

	virtual void SetRuntimeVersions(const TSharedPtr<TMap<FString, FString>>&) = 0;

	// totalMemory?: number;

	virtual const TSharedPtr<uint64> GetTotalMemory() const = 0;

	virtual void SetTotalMemory(const TSharedPtr<uint64>&) = 0;
};
