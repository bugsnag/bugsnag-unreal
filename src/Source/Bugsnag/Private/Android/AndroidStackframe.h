// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "BugsnagStackframe.h"
#include "JavaObjectWrapper.h"

class FAndroidStackframe : public IBugsnagStackframe, FJavaObjectWrapper
{
public:
	using FJavaObjectWrapper::FJavaObjectWrapper;
	using FJavaObjectWrapper::GetJavaObject;

	TOptional<FString> GetFile() const override
	{
		ReturnStringFieldOptional(JavaObject, Cache->StackframeGetFile);
	}

	void SetFile(const TOptional<FString>& Value) override
	{
		SetStringField(Cache->StackframeSetFile, true, Value);
	}

	bool GetInProject() const override
	{
		jboolean jValue = (*Env).CallBooleanMethod(JavaObject, Cache->StackframeGetInProject);
		return FAndroidPlatformJNI::CheckAndClearException(Env)
				   ? false
				   : jValue == JNI_TRUE;
	}

	void SetInProject(bool Value) override
	{
		(*Env).CallVoidMethod(JavaObject, Cache->StackframeSetInProject, Value ? JNI_TRUE : JNI_FALSE);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}

	TOptional<int32> GetLineNumber() const override
	{
		return GetIntObjectField<int32>(Cache->StackframeGetLineNumber);
	}

	void SetLineNumber(const TOptional<int32>& Line) override
	{
		SetIntObjectField(Cache->StackframeSetLineNumber, false, Line);
	}

	TOptional<FString> GetMethod() const override
	{
		ReturnStringFieldOptional(JavaObject, Cache->StackframeGetMethod);
	}

	void SetMethod(const TOptional<FString>& Value) override
	{
		SetStringField(Cache->StackframeSetMethod, true, Value);
	}

	TOptional<EBugsnagErrorType> GetType() const override
	{
		jobject jType = (*Env).CallObjectMethod(JavaObject, Cache->StackframeGetType);
		ReturnValueOnException(Env, TOptional<EBugsnagErrorType>());
		ReturnValueOnFail(jType, TOptional<EBugsnagErrorType>());
		return FAndroidPlatformJNI::ParseErrorType(Env, Cache, jType);
	}
};
