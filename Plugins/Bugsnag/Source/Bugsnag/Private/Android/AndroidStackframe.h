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
		jobject jLineNumber = (*Env).CallObjectMethod(JavaObject, Cache->StackframeGetLineNumber);
		ReturnValueOnException(Env, TOptional<int32>());
		jint LineNumber = (*Env).CallIntMethod(jLineNumber, Cache->NumberIntValue);
		ReturnValueOnException(Env, TOptional<int32>());
		return LineNumber;
	}

	void SetLineNumber(const TOptional<int32>& Line) override
	{
		jobject jLine = FAndroidPlatformJNI::ParseInteger(Env, Cache, Line.IsSet() ? Line.GetValue() : 0);
		ReturnVoidOnException(Env);
		(*Env).CallVoidMethod(JavaObject, Cache->StackframeSetLineNumber, jLine);
		FAndroidPlatformJNI::CheckAndClearException(Env);
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
