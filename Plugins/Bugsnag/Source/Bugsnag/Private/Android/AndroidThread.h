// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "AndroidStackframe.h"
#include "BugsnagErrorType.h"
#include "BugsnagThread.h"
#include "JavaObjectWrapper.h"

class FAndroidThread : public IBugsnagThread, FJavaObjectWrapper
{
public:
	using FJavaObjectWrapper::FJavaObjectWrapper;

	bool GetErrorReportingThread() const override
	{
		jboolean jValue = (*Env).CallBooleanMethod(JavaObject, Cache->ThreadGetErrorReportingThread);
		return FAndroidPlatformJNI::CheckAndClearException(Env)
				   ? false
				   : jValue == JNI_TRUE;
	}

	FString GetId() const override
	{
		// Retrieve the thread ID as a Java string
		jstring jId = static_cast<jstring>((*Env).CallObjectMethod(JavaObject, Cache->ThreadGetId));
		if (FAndroidPlatformJNI::CheckAndClearException(Env) || !jId)
		{
			return TEXT("");
		}

		// Convert the Java string to an FString
		const char* nativeString = (*Env).GetStringUTFChars(jId, nullptr);
		FString Id = FString(UTF8_TO_TCHAR(nativeString));
		(*Env).ReleaseStringUTFChars(jId, nativeString);

		return Id;
	}

	void SetId(const FString& Value) override
	{
		// Convert the FString to a Java string
		jstring jId = FAndroidPlatformJNI::ParseFString(Env, Value);
		if (jId)
		{
			(*Env).CallVoidMethod(JavaObject, Cache->ThreadSetId, jId);
			FAndroidPlatformJNI::CheckAndClearException(Env);
			(*Env).DeleteLocalRef(jId); // Clean up local reference
		}
	}

	FString GetName() const override
	{
		ReturnStringField(JavaObject, Cache->ThreadGetName);
	}

	void SetName(const FString& Value) override
	{
		SetField(Cache->ThreadSetName, false, FAndroidPlatformJNI::ParseFString(Env, Value));
	}

	EBugsnagErrorType GetErrorType() const override
	{
		jobject jErrorType = (*Env).CallObjectMethod(JavaObject, Cache->ErrorGetType);
		ReturnValueOnException(Env, EBugsnagErrorType::C);
		return FAndroidPlatformJNI::ParseErrorType(Env, Cache, jErrorType);
	}

	TArray<TSharedRef<IBugsnagStackframe>> GetStacktrace() const override
	{
		return GetListItems<IBugsnagStackframe, FAndroidStackframe>(Cache->ThreadGetStacktrace);
	}

	void SetStacktrace(const TArray<TSharedRef<IBugsnagStackframe>>& Value) override
	{
		jobject jFrames = (*Env).NewObject(Cache->ArrayListClass, Cache->ArrayListConstructor);
		ReturnVoidOnException(Env);
		for (auto& Frame : Value)
		{
			(*Env).CallBooleanMethod(jFrames, Cache->ListAdd, (StaticCastSharedRef<FAndroidStackframe>(Frame))->GetJavaObject());
			FAndroidPlatformJNI::CheckAndClearException(Env);
		}
		(*Env).CallVoidMethod(JavaObject, Cache->ThreadSetStacktrace, jFrames);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}
};
