#pragma once

#include "AndroidApp.h"
#include "AndroidDevice.h"
#include "BugsnagSession.h"
#include "JavaObjectWrapper.h"

class FAndroidSession : public IBugsnagSession, FJavaObjectWrapper
{
public:
	using FJavaObjectWrapper::FJavaObjectWrapper;

	const FString GetId() const
	{
		jobject jId = (*Env).CallObjectMethod(JavaObject, Cache->SessionGetId);
		FAndroidPlatformJNI::CheckAndClearException(Env);
		return FAndroidPlatformJNI::ParseJavaString(Env, Cache, jId);
	}

	void SetId(const FString& Id)
	{
		jstring jId = FAndroidPlatformJNI::ParseFString(Env, Id);
		if (jId)
		{
			(*Env).CallVoidMethod(JavaObject, Cache->SessionSetId, jId);
			FAndroidPlatformJNI::CheckAndClearException(Env);
		}
	}

	const FDateTime GetStartedAt() const
	{
		return GetDateField(Cache->SessionGetStartedAt);
	}

	void SetStartedAt(const FDateTime& StartedAt)
	{
		jobject jStartedAt = FAndroidPlatformJNI::ParseJavaDate(Env, Cache, StartedAt);
		if (jStartedAt)
		{
			(*Env).CallVoidMethod(JavaObject, Cache->SessionSetStartedAt, jStartedAt);
			FAndroidPlatformJNI::CheckAndClearException(Env);
		}
	}

	TSharedRef<IBugsnagApp> GetApp()
	{
		jobject jApp = (*Env).CallObjectMethod(JavaObject, Cache->SessionGetApp);
		FAndroidPlatformJNI::CheckAndClearException(Env);
		return MakeShared<FAndroidApp>(Env, Cache, jApp);
	}

	TSharedRef<IBugsnagDevice> GetDevice()
	{
		jobject jDevice = (*Env).CallObjectMethod(JavaObject, Cache->SessionGetDevice);
		FAndroidPlatformJNI::CheckAndClearException(Env);
		return MakeShared<FAndroidDevice>(Env, Cache, jDevice);
	}

	const FBugsnagUser GetUser() const
	{
		jobject jUser = (*Env).CallObjectMethod(JavaObject, Cache->SessionGetUser);
		return FAndroidPlatformJNI::ParseJavaUser(Env, Cache, jUser);
	}

	void SetUser(const TSharedPtr<FString>& Id, const TSharedPtr<FString>& Email, const TSharedPtr<FString>& Name)
	{
		jstring jId = Id.IsValid() ? FAndroidPlatformJNI::ParseFString(Env, *Id) : nullptr;
		jstring jName = Name.IsValid() ? FAndroidPlatformJNI::ParseFString(Env, *Name) : nullptr;
		jstring jEmail = Email.IsValid() ? FAndroidPlatformJNI::ParseFString(Env, *Email) : nullptr;
		(*Env).CallVoidMethod(JavaObject, Cache->SessionSetUser, jId, jEmail, jName);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}
};
