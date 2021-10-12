#pragma once

#include <jni.h>

#include "BugsnagBreadcrumb.h"
#include "JNIUtilities.h"

class FAndroidBreadcrumb : public IBugsnagBreadcrumb
{
public:
	static TSharedRef<FAndroidBreadcrumb> From(JNIEnv* Env, const JNIReferenceCache* Cache, jobject Crumb)
	{
		return MakeShared<FAndroidBreadcrumb>(Env, Cache, Crumb);
	}

	FAndroidBreadcrumb(JNIEnv* Env, const JNIReferenceCache* Cache, jobject Crumb)
		: Crumb(Crumb)
		, Cache(Cache)
		, Env(Env)
	{
	}

	const FDateTime GetTimestamp() const override
	{
		jobject jTimestamp = (*Env).CallObjectMethod(Crumb, Cache->BreadcrumbGetTimestamp);
		if (FAndroidPlatformJNI::CheckAndClearException(Env))
		{
			return FDateTime(0);
		}
		jlong milliseconds = (*Env).CallLongMethod(jTimestamp, Cache->DateGetTime);
		if (FAndroidPlatformJNI::CheckAndClearException(Env))
		{
			return FDateTime(0);
		}
		return FDateTime(FDateTime(1970, 1, 1).GetTicks() + milliseconds * ETimespan::TicksPerMillisecond);
	}

	const FString GetMessage() const override
	{
		jobject jMessage = (*Env).CallObjectMethod(Crumb, Cache->BreadcrumbGetMessage);
		if (FAndroidPlatformJNI::CheckAndClearException(Env))
		{
			return TEXT("");
		}
		const char* Message = (*Env).GetStringUTFChars((jstring)jMessage, nullptr);
		if (FAndroidPlatformJNI::CheckAndClearException(Env))
		{
			return TEXT("");
		}
		return UTF8_TO_TCHAR(Message);
	}

	void SetMessage(const FString& Message) override
	{
		jstring jMessage = FAndroidPlatformJNI::ParseFString(Env, Message);
		if (jMessage)
		{
			(*Env).CallVoidMethod(Crumb, Cache->BreadcrumbSetMessage, jMessage);
			FAndroidPlatformJNI::CheckAndClearException(Env);
		}
	}

	EBugsnagBreadcrumbType GetType() const override
	{
		jobject jType = (*Env).CallObjectMethod(Crumb, Cache->BreadcrumbGetType);
		if (FAndroidPlatformJNI::CheckAndClearException(Env))
		{
			return EBugsnagBreadcrumbType::Manual;
		}
		jobject jName = (*Env).CallObjectMethod(jType, Cache->EnumGetName);
		if (FAndroidPlatformJNI::CheckAndClearException(Env))
		{
			return EBugsnagBreadcrumbType::Manual;
		}
		const char* Name = (*Env).GetStringUTFChars((jstring)jName, nullptr);
		if (FAndroidPlatformJNI::CheckAndClearException(Env))
		{
			return EBugsnagBreadcrumbType::Manual;
		}
		switch (Name[0])
		{
		case 'E':
			return EBugsnagBreadcrumbType::Error;
		case 'L':
			return EBugsnagBreadcrumbType::Log;
		case 'N':
			return EBugsnagBreadcrumbType::Navigation;
		case 'R':
			return EBugsnagBreadcrumbType::Request;
		case 'S':
			return EBugsnagBreadcrumbType::State;
		case 'U':
			return EBugsnagBreadcrumbType::User;
		case 'M':
		default:
			return EBugsnagBreadcrumbType::Manual;
		}
	}

	void SetType(EBugsnagBreadcrumbType Type) override
	{
		jobject jType = FAndroidPlatformJNI::ParseBreadcrumbType(Env, Cache, Type);
		if (jType)
		{
			(*Env).CallVoidMethod(Crumb, Cache->BreadcrumbSetType, jType);
			FAndroidPlatformJNI::CheckAndClearException(Env);
		}
	}

	TSharedPtr<FJsonObject> GetMetadata() const override
	{
		jobject jMetadata = (*Env).CallObjectMethod(Crumb, Cache->BreadcrumbGetMetadata);
		if (FAndroidPlatformJNI::CheckAndClearException(Env))
		{
			return MakeShareable(new FJsonObject);
		}
		return FAndroidPlatformJNI::ConvertJavaMapToJson(Env, Cache, jMetadata);
	}

	void SetMetadata(const TSharedPtr<FJsonObject>& Metadata) override
	{
		auto jMetadata = FAndroidPlatformJNI::ParseJsonObject(Env, Cache, Metadata);
		if (jMetadata)
		{
			(*Env).CallVoidMethod(Crumb, Cache->BreadcrumbSetMetadata, jMetadata);
		}
	}

private:
	JNIEnv* Env;
	const JNIReferenceCache* Cache;
	const jobject Crumb;
};
