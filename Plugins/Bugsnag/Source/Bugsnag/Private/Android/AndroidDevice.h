#pragma once

#include <jni.h>

#include "BugsnagDevice.h"
#include "Containers/Array.h"
#include "Dom/JsonObject.h"
#include "JNIUtilities.h"
#include "Shorthand.h"
#include "Templates/SharedPointer.h"

class FAndroidDevice : virtual public IBugsnagDevice
{
public:
	static TSharedRef<FAndroidDevice> From(JNIEnv* Env, const JNIReferenceCache* Cache, jobject Device)
	{
		return MakeShared<FAndroidDevice>(Env, Cache, Device);
	}

	FAndroidDevice(JNIEnv* Env, const JNIReferenceCache* Cache, jobject Device)
		: Device(Device)
		, Cache(Cache)
		, Env(Env)
	{
	}

	const TSharedPtr<TArray<FString>> GetCpuAbi() const
	{
		TSharedPtr<TArray<FString>> ABIs = MakeShareable(new TArray<FString>);
		jobjectArray jABIs = (jobjectArray)(*Env).CallObjectMethod(Device, Cache->DeviceGetCpuAbi);
		if (FAndroidPlatformJNI::CheckAndClearException(Env))
		{
			return ABIs;
		}
		jsize length = (*Env).GetArrayLength(jABIs);
		if (FAndroidPlatformJNI::CheckAndClearException(Env))
		{
			return ABIs;
		}
		for (jsize Index = 0; Index < length; Index++)
		{
			jobject jABI = (*Env).GetObjectArrayElement(jABIs, Index);
			if (FAndroidPlatformJNI::CheckAndClearException(Env))
			{
				return ABIs;
			}
			ABIs->Add(FAndroidPlatformJNI::ParseJavaString(Env, Cache, jABI));
		}
		return ABIs;
	}

	void SetCpuAbi(const TSharedPtr<TArray<FString>>& Value)
	{
		jobjectArray jABIs = nullptr;
		if (Value.IsValid())
		{
			int32 Length = (*Value).Num();
			jABIs = (*Env).NewObjectArray(Length, Cache->StringClass, NULL);
			ReturnVoidOnException(Env);
			for (int32 Index = 0; Index < Length; Index++)
			{
				jstring jABI = FAndroidPlatformJNI::ParseFString(Env, (*Value)[Index]);
				if (jABI)
				{
					(*Env).SetObjectArrayElement(jABIs, Index, jABI);
					FAndroidPlatformJNI::CheckAndClearException(Env);
				}
			}
		}
		(*Env).CallVoidMethod(Device, Cache->DeviceSetCpuAbi, jABIs);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}

	const TSharedPtr<FString> GetId() const
	{
		ReturnStringFieldPtr(Device, Cache->DeviceGetId);
	}

	void SetId(const TSharedPtr<FString>& Value)
	{
		FAndroidPlatformJNI::SetStringValue(Env, Device, Cache->DeviceSetId, true, Value);
	}

	TSharedPtr<bool> GetJailbroken() const
	{
		jboolean jValue = (*Env).CallBooleanMethod(Device, Cache->DeviceGetJailbroken);
		return FAndroidPlatformJNI::CheckAndClearException(Env)
				   ? nullptr
				   : MakeShareable(new bool(jValue == JNI_TRUE));
	}

	void SetJailbroken(const TSharedPtr<bool>& Value)
	{
		if (Value.IsValid())
		{
			jboolean jValue = *Value ? JNI_TRUE : JNI_FALSE;
			(*Env).CallVoidMethod(Device, Cache->DeviceSetJailbroken, jValue);
		}
		else
		{
			(*Env).CallVoidMethod(Device, Cache->DeviceSetJailbroken, nullptr);
		}
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}

	const TSharedPtr<FString> GetLocale() const
	{
		ReturnStringFieldPtr(Device, Cache->DeviceGetLocale);
	}

	void SetLocale(const TSharedPtr<FString>& Value)
	{
		FAndroidPlatformJNI::SetStringValue(Env, Device, Cache->DeviceSetLocale, true, Value);
	}

	const TSharedPtr<FString> GetManufacturer() const
	{
		ReturnStringFieldPtr(Device, Cache->DeviceGetManufacturer);
	}

	void SetManufacturer(const TSharedPtr<FString>& Value)
	{
		FAndroidPlatformJNI::SetStringValue(Env, Device, Cache->DeviceSetManufacturer, true, Value);
	}

	const TSharedPtr<FString> GetModel() const
	{
		ReturnStringFieldPtr(Device, Cache->DeviceGetModel);
	}

	void SetModel(const TSharedPtr<FString>& Value)
	{
		FAndroidPlatformJNI::SetStringValue(Env, Device, Cache->DeviceSetModel, true, Value);
	}

	const TSharedPtr<FString> GetOsName() const
	{
		ReturnStringFieldPtr(Device, Cache->DeviceGetOsName);
	}

	void SetOsName(const TSharedPtr<FString>& Value)
	{
		FAndroidPlatformJNI::SetStringValue(Env, Device, Cache->DeviceSetOsName, true, Value);
	}

	const TSharedPtr<FString> GetOsVersion() const
	{
		ReturnStringFieldPtr(Device, Cache->DeviceGetOsVersion);
	}

	void SetOsVersion(const TSharedPtr<FString>& Value)
	{
		FAndroidPlatformJNI::SetStringValue(Env, Device, Cache->DeviceSetOsVersion, true, Value);
	}

	const TSharedPtr<TMap<FString, FString>> GetRuntimeVersions() const
	{
		TSharedPtr<TMap<FString, FString>> RuntimeVersions = MakeShareable(new TMap<FString, FString>);
		jobject jRuntimeVersions = (*Env).CallObjectMethod(Device, Cache->DeviceGetRuntimeVersions);
		ReturnValueOnException(Env, RuntimeVersions);
		jint jSize = (*Env).CallIntMethod(jRuntimeVersions, Cache->MapSize);
		ReturnValueOnException(Env, RuntimeVersions);
		jobject jKeySet = (*Env).CallObjectMethod(jRuntimeVersions, Cache->MapKeySet);
		ReturnValueOnException(Env, RuntimeVersions);
		jobject jKeyList = (*Env).NewObject(Cache->ArrayListClass, Cache->ArrayListCollectionConstructor, jKeySet);
		ReturnValueOnException(Env, RuntimeVersions);
		for (jint Index = 0; Index < jSize; Index++)
		{
			jobject jVersionName = (*Env).CallObjectMethod(jKeyList, Cache->ArrayListGet, Index);
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
			(*Env).CallVoidMethod(Device, Cache->DeviceSetRuntimeVersions, nullptr);
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
			(*Env).CallVoidMethod(Device, Cache->DeviceSetRuntimeVersions, jValues);
			FAndroidPlatformJNI::CheckAndClearException(Env);
		}
	}

	const TSharedPtr<uint64> GetTotalMemory() const
	{
		jlong jTotalMem = (*Env).CallLongMethod(Device, Cache->DeviceGetTotalMemory);
		return FAndroidPlatformJNI::CheckAndClearException(Env)
				   ? nullptr
				   : MakeShareable(new uint64(jTotalMem));
	}

	void SetTotalMemory(const TSharedPtr<uint64>& Value)
	{
		jobject jTotalMem = FAndroidPlatformJNI::ParseInteger(Env, Cache, Value.IsValid() ? *Value : 0);
		if (jTotalMem)
		{
			(*Env).CallVoidMethod(Device, Cache->DeviceSetTotalMemory, jTotalMem);
			FAndroidPlatformJNI::CheckAndClearException(Env);
		}
	}

	// -- unsupported fields

	const TSharedPtr<FString> GetModelNumber() const
	{
		return nullptr;
	}

	void SetModelNumber(const TSharedPtr<FString>& Value)
	{
	}

private:
	JNIEnv* Env;
	const JNIReferenceCache* Cache;
	const jobject Device;
};
