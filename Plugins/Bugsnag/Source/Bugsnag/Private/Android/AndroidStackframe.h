#pragma once

#include "BugsnagStackframe.h"
#include "JavaObjectWrapper.h"

class FAndroidStackframe : public IBugsnagStackframe, FJavaObjectWrapper
{
public:
	using FJavaObjectWrapper::FJavaObjectWrapper;
	using FJavaObjectWrapper::GetJavaObject;

	TSharedPtr<FString> GetFile() const override
	{
		ReturnStringFieldPtr(JavaObject, Cache->StackframeGetFile);
	}

	void SetFile(const TSharedPtr<FString>& Value) override
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

	TSharedPtr<int32> GetLineNumber() const override
	{
		jobject jLineNumber = (*Env).CallObjectMethod(JavaObject, Cache->StackframeGetLineNumber);
		ReturnNullOnException(Env);
		jint LineNumber = (*Env).CallIntMethod(jLineNumber, Cache->NumberIntValue);
		ReturnNullOnException(Env);
		return MakeShareable(new int32(LineNumber));
	}

	void SetLineNumber(const TSharedPtr<int32>& Line) override
	{
		jobject jLine = FAndroidPlatformJNI::ParseInteger(Env, Cache, Line.IsValid() ? *Line : 0);
		ReturnVoidOnException(Env);
		(*Env).CallVoidMethod(JavaObject, Cache->StackframeSetLineNumber, jLine);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}

	TSharedPtr<FString> GetMethod() const override
	{
		ReturnStringFieldPtr(JavaObject, Cache->StackframeGetMethod);
	}

	void SetMethod(const TSharedPtr<FString>& Value) override
	{
		SetStringField(Cache->StackframeSetMethod, true, Value);
	}

	TSharedPtr<EBugsnagErrorType> GetType() const override
	{
		jobject jType = (*Env).CallObjectMethod(JavaObject, Cache->StackframeGetType);
		ReturnNullOnException(Env);
		ReturnNullOnFail(jType);
		return MakeShareable(new EBugsnagErrorType(FAndroidPlatformJNI::ParseErrorType(Env, Cache, jType)));
	}
};
