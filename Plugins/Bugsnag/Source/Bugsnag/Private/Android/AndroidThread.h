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
		auto Id = (*Env).CallLongMethod(JavaObject, Cache->ThreadGetId);
		FAndroidPlatformJNI::CheckAndClearException(Env);
		return Id ? FString::Printf(TEXT("{Id}"), Id) : TEXT("");
	}

	void SetId(const FString& Value) override
	{
		int64 Id = FCString::Atoi64(*Value);
		if (Id)
		{
			(*Env).CallVoidMethod(JavaObject, Cache->ThreadSetId, Id);
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
		jobject jErrorType = (*Env).CallObjectMethod(JavaObject, Cache->ThreadGetType);
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
			(*Env).CallVoidMethod(jFrames, Cache->ListAdd, (StaticCastSharedRef<FAndroidStackframe>(Frame))->GetJavaObject());
			FAndroidPlatformJNI::CheckAndClearException(Env);
		}
		(*Env).CallVoidMethod(JavaObject, Cache->ThreadSetStacktrace, jFrames);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}
};
