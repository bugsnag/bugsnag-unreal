// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "AndroidStackframe.h"
#include "BugsnagError.h"
#include "JavaObjectWrapper.h"

class FAndroidError : public IBugsnagError, FJavaObjectWrapper
{
public:
	using FJavaObjectWrapper::FJavaObjectWrapper;

	FString GetErrorClass() const override
	{
		ReturnStringField(JavaObject, Cache->ErrorGetErrorClass);
	}

	void SetErrorClass(const FString& Value) override
	{
		SetField(Cache->ErrorSetErrorClass, false, FAndroidPlatformJNI::ParseFString(Env, Value));
	}

	FString GetErrorMessage() const override
	{
		ReturnStringField(JavaObject, Cache->ErrorGetErrorMessage);
	}

	void SetErrorMessage(const FString& Value) override
	{
		SetField(Cache->ErrorSetErrorMessage, false, FAndroidPlatformJNI::ParseFString(Env, Value));
	}

	EBugsnagErrorType GetErrorType() const override
	{
		jobject jErrorType = (*Env).CallObjectMethod(JavaObject, Cache->ErrorGetType);
		ReturnValueOnException(Env, EBugsnagErrorType::C);
		return FAndroidPlatformJNI::ParseErrorType(Env, Cache, jErrorType);
	}

	TArray<TSharedRef<IBugsnagStackframe>> GetStacktrace() const override
	{
		return GetListItems<IBugsnagStackframe, FAndroidStackframe>(Cache->ErrorGetStacktrace);
	}

	void SetStacktrace(const TArray<TSharedRef<IBugsnagStackframe>>& Value) override
	{
		// replacing stacktrace in-place (as there is no setter)
		jobject jFrames = (*Env).CallObjectMethod(JavaObject, Cache->ErrorGetStacktrace);
		ReturnVoidOnException(Env);
		(*Env).CallVoidMethod(jFrames, Cache->ListClear);
		ReturnVoidOnException(Env);
		for (auto& Frame : Value)
		{
			(*Env).CallBooleanMethod(jFrames, Cache->ListAdd, (StaticCastSharedRef<FAndroidStackframe>(Frame))->GetJavaObject());
			FAndroidPlatformJNI::CheckAndClearException(Env);
		}
	}
};
