#pragma once

#include <jni.h>

#include "BugsnagApp.h"
#include "JNIUtilities.h"
#include "Shorthand.h"

class FAndroidApp : virtual public IBugsnagApp
{
public:
	static TSharedRef<FAndroidApp> From(JNIEnv* Env, const JNIReferenceCache* Cache, jobject App)
	{
		return MakeShared<FAndroidApp>(Env, Cache, App);
	}

	FAndroidApp(JNIEnv* Env, const JNIReferenceCache* Cache, jobject App)
		: App(App)
		, Cache(Cache)
		, Env(Env)
	{
	}

	const TSharedPtr<FString> GetBinaryArch() const
	{
		ReturnStringFieldPtr(App, Cache->AppGetBinaryArch);
	}

	void SetBinaryArch(const TSharedPtr<FString>& Value)
	{
		FAndroidPlatformJNI::SetStringValue(Env, App, Cache->AppSetBinaryArch, true, Value);
	}

	const TSharedPtr<FString> GetBuildUuid() const
	{
		ReturnStringFieldPtr(App, Cache->AppGetBuildUuid);
	}

	void SetBuildUuid(const TSharedPtr<FString>& Value)
	{
		FAndroidPlatformJNI::SetStringValue(Env, App, Cache->AppSetBuildUuid, true, Value);
	}

	const TSharedPtr<FString> GetId() const
	{
		ReturnStringFieldPtr(App, Cache->AppGetId);
	}

	void SetId(const TSharedPtr<FString>& Value)
	{
		FAndroidPlatformJNI::SetStringValue(Env, App, Cache->AppSetId, true, Value);
	}

	const TSharedPtr<FString> GetReleaseStage() const
	{
		ReturnStringFieldPtr(App, Cache->AppGetReleaseStage);
	}

	void SetReleaseStage(const TSharedPtr<FString>& Value)
	{
		FAndroidPlatformJNI::SetStringValue(Env, App, Cache->AppSetReleaseStage, true, Value);
	}

	const TSharedPtr<FString> GetType() const
	{
		ReturnStringFieldPtr(App, Cache->AppGetType);
	}

	void SetType(const TSharedPtr<FString>& Value)
	{
		FAndroidPlatformJNI::SetStringValue(Env, App, Cache->AppSetType, true, Value);
	}

	const TSharedPtr<FString> GetVersion() const
	{
		ReturnStringFieldPtr(App, Cache->AppGetVersion);
	}

	void SetVersion(const TSharedPtr<FString>& Value)
	{
		FAndroidPlatformJNI::SetStringValue(Env, App, Cache->AppSetVersion, true, Value);
	}

	const TSharedPtr<int64> GetVersionCode() const
	{
		jlong VersionCode = (*Env).CallLongMethod(App, Cache->AppGetVersionCode);
		return FAndroidPlatformJNI::CheckAndClearException(Env)
				   ? nullptr
				   : MakeShareable(new int64(VersionCode));
	}

	void SetVersionCode(const TSharedPtr<int64>& Value)
	{
		jobject jVersionCode = nullptr;
		if (Value.IsValid())
		{
			jVersionCode = FAndroidPlatformJNI::ParseInteger(Env, Cache, *Value);
			if (!jVersionCode)
			{
				return; // avoid clearing the value when intended to set a value
			}
		}
		(*Env).CallVoidMethod(App, Cache->AppSetVersionCode, jVersionCode);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}

	// -- unsupported fields

	const TSharedPtr<FString> GetBundleVersion() const
	{
		return nullptr;
	}

	void SetBundleVersion(const TSharedPtr<FString>& Value)
	{
	}

	const TSharedPtr<FString> GetDsymUuid() const
	{
		return nullptr;
	}

	void SetDsymUuid(const TSharedPtr<FString>& Value)
	{
	}

private:
	JNIEnv* Env;
	const JNIReferenceCache* Cache;
	const jobject App;
};
