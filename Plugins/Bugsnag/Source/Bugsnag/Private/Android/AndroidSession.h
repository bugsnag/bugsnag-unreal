#pragma once

#include <jni.h>

#include "AndroidApp.h"
#include "AndroidDevice.h"
#include "BugsnagSession.h"
#include "JNIUtilities.h"

class FAndroidSession : public IBugsnagSession
{
public:
	static TSharedRef<FAndroidSession> From(JNIEnv* Env, const JNIReferenceCache* Cache, jobject Session)
	{
		return MakeShared<FAndroidSession>(Env, Cache, Session);
	}

	FAndroidSession(JNIEnv* Env, const JNIReferenceCache* Cache, jobject Session)
		: Session(Session)
		, Cache(Cache)
		, Env(Env)
	{
	}

	const FString GetId() const
	{
		jobject jId = (*Env).CallObjectMethod(Session, Cache->SessionGetId);
		FAndroidPlatformJNI::CheckAndClearException(Env);
		return FAndroidPlatformJNI::ParseJavaString(Env, Cache, jId);
	}

	void SetId(const FString& Id)
	{
		jstring jId = FAndroidPlatformJNI::ParseFString(Env, Id);
		if (jId)
		{
			(*Env).CallVoidMethod(Session, Cache->SessionSetId, jId);
			FAndroidPlatformJNI::CheckAndClearException(Env);
		}
	}

	const FDateTime GetStartedAt() const
	{
		jobject jTimestamp = (*Env).CallObjectMethod(Session, Cache->SessionGetStartedAt);
		FAndroidPlatformJNI::CheckAndClearException(Env);
		return FAndroidPlatformJNI::ParseDateTime(Env, Cache, jTimestamp);
	}

	void SetStartedAt(const FDateTime& StartedAt)
	{
		jobject jStartedAt = FAndroidPlatformJNI::ParseJavaDate(Env, Cache, StartedAt);
		if (jStartedAt)
		{
			(*Env).CallVoidMethod(Session, Cache->SessionSetStartedAt, jStartedAt);
			FAndroidPlatformJNI::CheckAndClearException(Env);
		}
	}

	TSharedRef<IBugsnagApp> GetApp()
	{
		jobject jApp = (*Env).CallObjectMethod(Session, Cache->SessionGetApp);
		FAndroidPlatformJNI::CheckAndClearException(Env);
		return FAndroidApp::From(Env, Cache, jApp);
	}

	TSharedRef<IBugsnagDevice> GetDevice()
	{
		jobject jDevice = (*Env).CallObjectMethod(Session, Cache->SessionGetDevice);
		FAndroidPlatformJNI::CheckAndClearException(Env);
		return FAndroidDevice::From(Env, Cache, jDevice);
	}

	const FBugsnagUser GetUser() const
	{
		jobject jUser = (*Env).CallObjectMethod(Session, Cache->SessionGetUser);
		if (FAndroidPlatformJNI::CheckAndClearException(Env))
		{
			return FBugsnagUser(
				MakeShareable(new FString("")),
				MakeShareable(new FString("")),
				MakeShareable(new FString("")));
		}
		jobject jId = (*Env).CallObjectMethod(jUser, Cache->UserGetId);
		FAndroidPlatformJNI::CheckAndClearException(Env);
		FString Id = FAndroidPlatformJNI::ParseJavaString(Env, Cache, jId);
		jobject jName = (*Env).CallObjectMethod(jUser, Cache->UserGetName);
		FAndroidPlatformJNI::CheckAndClearException(Env);
		FString Name = FAndroidPlatformJNI::ParseJavaString(Env, Cache, jName);
		jobject jEmail = (*Env).CallObjectMethod(jUser, Cache->UserGetEmail);
		FAndroidPlatformJNI::CheckAndClearException(Env);
		FString Email = FAndroidPlatformJNI::ParseJavaString(Env, Cache, jEmail);
		return FBugsnagUser(
			MakeShared<FString>(Id),
			MakeShared<FString>(Email),
			MakeShared<FString>(Name));
	}

	void SetUser(const TSharedPtr<FString>& Id, const TSharedPtr<FString>& Email, const TSharedPtr<FString>& Name)
	{
		jstring jId = Id.IsValid() ? FAndroidPlatformJNI::ParseFString(Env, *Id) : nullptr;
		jstring jName = Name.IsValid() ? FAndroidPlatformJNI::ParseFString(Env, *Name) : nullptr;
		jstring jEmail = Email.IsValid() ? FAndroidPlatformJNI::ParseFString(Env, *Email) : nullptr;
		(*Env).CallVoidMethod(Session, Cache->SessionSetUser, jId, jEmail, jName);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}

private:
	JNIEnv* Env;
	const JNIReferenceCache* Cache;
	const jobject Session;
};
