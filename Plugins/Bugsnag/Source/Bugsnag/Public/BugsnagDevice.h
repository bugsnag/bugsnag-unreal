#pragma once

class BUGSNAG_API IBugsnagDevice
{
public:
	// cpuAbi?: string[];

	virtual const TSharedPtr<TArray<FString>> GetCpuAbi() const = 0;

	virtual void SetCpuAbi(const TSharedPtr<TArray<FString>>&) = 0;

	// id?: string;

	virtual const TOptional<FString> GetId() const = 0;

	virtual void SetId(const TOptional<FString>&) = 0;

	// jailbroken?: boolean;

	virtual TOptional<bool> GetJailbroken() const = 0;

	virtual void SetJailbroken(const TOptional<bool>&) = 0;

	// locale?: string;

	virtual const TOptional<FString> GetLocale() const = 0;

	virtual void SetLocale(const TOptional<FString>&) = 0;

	// modelNumber?: string;

	virtual const TOptional<FString> GetModelNumber() const = 0;

	virtual void SetModelNumber(const TOptional<FString>&) = 0;

	// manufacturer?: string;

	virtual const TOptional<FString> GetManufacturer() const = 0;

	virtual void SetManufacturer(const TOptional<FString>&) = 0;

	// model?: string;

	virtual const TOptional<FString> GetModel() const = 0;

	virtual void SetModel(const TOptional<FString>&) = 0;

	// osName?: string;

	virtual const TOptional<FString> GetOsName() const = 0;

	virtual void SetOsName(const TOptional<FString>&) = 0;

	// osVersion?: string;

	virtual const TOptional<FString> GetOsVersion() const = 0;

	virtual void SetOsVersion(const TOptional<FString>&) = 0;

	// runtimeVersions?: map[string->string];

	virtual const TSharedPtr<TMap<FString, FString>> GetRuntimeVersions() const = 0;

	virtual void SetRuntimeVersions(const TSharedPtr<TMap<FString, FString>>&) = 0;

	// totalMemory?: number;

	virtual const TOptional<uint64> GetTotalMemory() const = 0;

	virtual void SetTotalMemory(const TOptional<uint64>&) = 0;
};
