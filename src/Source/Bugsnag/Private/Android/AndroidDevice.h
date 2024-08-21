// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "BugsnagDevice.h"
#include "JavaObjectWrapper.h"

class FAndroidDevice : virtual public IBugsnagDevice, public FJavaObjectWrapper
{
public:
	using FJavaObjectWrapper::FJavaObjectWrapper;

	const TSharedPtr<TArray<FString>> GetCpuAbi() const
	{
		return GetStringArrayField(Cache->DeviceGetCpuAbi);
	}

	void SetCpuAbi(const TSharedPtr<TArray<FString>>& Value)
	{
		SetStringArrayField(Cache->DeviceSetCpuAbi, Value);
	}

	const TOptional<FString> GetId() const
	{
		ReturnStringFieldOptional(JavaObject, Cache->DeviceGetId);
	}

	void SetId(const TOptional<FString>& Value)
	{
		SetStringField(Cache->DeviceSetId, true, Value);
	}

	TOptional<bool> GetJailbroken() const
	{
		return GetBoolObjectField(Cache->DeviceGetJailbroken);
	}

	void SetJailbroken(const TOptional<bool>& Value)
	{
		SetBoolObjectField(Cache->DeviceSetJailbroken, true, Value);
	}

	const TOptional<FString> GetLocale() const
	{
		ReturnStringFieldOptional(JavaObject, Cache->DeviceGetLocale);
	}

	void SetLocale(const TOptional<FString>& Value)
	{
		SetStringField(Cache->DeviceSetLocale, true, Value);
	}

	const TOptional<FString> GetManufacturer() const
	{
		ReturnStringFieldOptional(JavaObject, Cache->DeviceGetManufacturer);
	}

	void SetManufacturer(const TOptional<FString>& Value)
	{
		SetStringField(Cache->DeviceSetManufacturer, true, Value);
	}

	const TOptional<FString> GetModel() const
	{
		ReturnStringFieldOptional(JavaObject, Cache->DeviceGetModel);
	}

	void SetModel(const TOptional<FString>& Value)
	{
		SetStringField(Cache->DeviceSetModel, true, Value);
	}

	const TOptional<FString> GetOsName() const
	{
		ReturnStringFieldOptional(JavaObject, Cache->DeviceGetOsName);
	}

	void SetOsName(const TOptional<FString>& Value)
	{
		SetStringField(Cache->DeviceSetOsName, true, Value);
	}

	const TOptional<FString> GetOsVersion() const
	{
		ReturnStringFieldOptional(JavaObject, Cache->DeviceGetOsVersion);
	}

	void SetOsVersion(const TOptional<FString>& Value)
	{
		SetStringField(Cache->DeviceSetOsVersion, true, Value);
	}

	const TSharedPtr<TMap<FString, FString>> GetRuntimeVersions() const
	{
		TSharedPtr<TMap<FString, FString>> RuntimeVersions = MakeShareable(new TMap<FString, FString>);
		jobject jRuntimeVersions = (*Env).CallObjectMethod(JavaObject, Cache->DeviceGetRuntimeVersions);
		ReturnValueOnException(Env, RuntimeVersions);
		jint jSize = (*Env).CallIntMethod(jRuntimeVersions, Cache->MapSize);
		ReturnValueOnException(Env, RuntimeVersions);
		jobject jKeySet = (*Env).CallObjectMethod(jRuntimeVersions, Cache->MapKeySet);
		ReturnValueOnException(Env, RuntimeVersions);
		jobject jKeyList = (*Env).NewObject(Cache->ArrayListClass, Cache->ArrayListCollectionConstructor, jKeySet);
		ReturnValueOnException(Env, RuntimeVersions);
		for (jint Index = 0; Index < jSize; Index++)
		{
			jobject jVersionName = (*Env).CallObjectMethod(jKeyList, Cache->ListGet, Index);
			ContinueOnException(Env);
			FString VersionName = FAndroidPlatformJNI::ParseJavaString(Env, Cache, jVersionName);
			ContinueOnException(Env);
			jobject jVersionValue = (*Env).CallObjectMethod(jRuntimeVersions, Cache->HashMapGet, jVersionName);
			ContinueOnException(Env);
			FString VersionValue = FAndroidPlatformJNI::ParseJavaString(Env, Cache, jVersionValue);
			ContinueOnException(Env);
			RuntimeVersions->Add(VersionName, VersionValue);
		}
		return RuntimeVersions;
	}

	void SetRuntimeVersions(const TSharedPtr<TMap<FString, FString>>& Value)
	{
		if (!Value.IsValid())
		{
			(*Env).CallVoidMethod(JavaObject, Cache->DeviceSetRuntimeVersions, nullptr);
			FAndroidPlatformJNI::CheckAndClearException(Env);
			return;
		}
		TSharedRef<FJsonObject> Values = MakeShared<FJsonObject>();
		for (const TPair<FString, FString>& Pair : *Value)
		{
			Values->SetStringField(Pair.Key, Pair.Value);
		}

		jobject jValues = FAndroidPlatformJNI::ParseJsonObject(Env, Cache, Values);
		if (jValues)
		{
			(*Env).CallVoidMethod(JavaObject, Cache->DeviceSetRuntimeVersions, jValues);
			FAndroidPlatformJNI::CheckAndClearException(Env);
		}
	}

	const TOptional<uint64> GetTotalMemory() const
	{
		return GetLongObjectField<uint64>(Cache->DeviceGetTotalMemory);
	}

	void SetTotalMemory(const TOptional<uint64>& Value)
	{
		SetLongObjectField(Cache->DeviceSetTotalMemory, true, Value);
	}

	// -- unsupported fields

	const TOptional<FString> GetModelNumber() const
	{
		return TOptional<FString>();
	}

	void SetModelNumber(const TOptional<FString>& Value)
	{
	}
};
