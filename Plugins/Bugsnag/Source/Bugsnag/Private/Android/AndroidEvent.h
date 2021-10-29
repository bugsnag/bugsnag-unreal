#pragma once

#include "CoreMinimal.h"

#include "AndroidAppWithState.h"
#include "AndroidBreadcrumb.h"
#include "AndroidDeviceWithState.h"
#include "AndroidError.h"
#include "AndroidThread.h"
#include "BugsnagEvent.h"
#include "BugsnagUser.h"
#include "JavaObjectWrapper.h"

class FAndroidEvent : public IBugsnagEvent, FJavaObjectWrapper
{
public:
	using FJavaObjectWrapper::FJavaObjectWrapper;

	void AddMetadata(const FString& Section, const TSharedRef<FJsonObject>& Metadata) override
	{
		jstring jSection = FAndroidPlatformJNI::ParseFString(Env, Section);
		ReturnVoidIf(!jSection);
		jobject jMetadata = FAndroidPlatformJNI::ParseJsonObject(Env, Cache, Metadata);
		ReturnVoidIf(!jMetadata);
		(*Env).CallVoidMethod(JavaObject, Cache->EventAddMetadataToSection, jSection, jMetadata);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}

	void AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value) override
	{
		jstring jSection = FAndroidPlatformJNI::ParseFString(Env, Section);
		ReturnVoidIf(!jSection);
		jstring jKey = FAndroidPlatformJNI::ParseFString(Env, Key);
		ReturnVoidIf(!jKey);
		TSharedPtr<FJsonObject> Container = MakeShareable(new FJsonObject);
		Container->SetField("Value", Value);
		jobject jContainer = FAndroidPlatformJNI::ParseJsonObject(Env, Cache, Container);
		ReturnVoidOnException(Env);
		jstring jValueKey = (*Env).NewStringUTF("Value");
		ReturnVoidOnException(Env);
		jobject jMetadata = (*Env).CallObjectMethod(jContainer, Cache->HashMapGet, jValueKey);
		ReturnVoidOnException(Env);
		(*Env).CallVoidMethod(JavaObject, Cache->EventAddMetadataValue, jSection, jKey, jMetadata);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}

	void ClearMetadata(const FString& Section) override
	{
		jstring jSection = FAndroidPlatformJNI::ParseFString(Env, Section);
		ReturnVoidIf(!jSection);
		(*Env).CallVoidMethod(JavaObject, Cache->EventClearMetadataSection, jSection);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}

	void ClearMetadata(const FString& Section, const FString& Key) override
	{
		jstring jSection = FAndroidPlatformJNI::ParseFString(Env, Section);
		ReturnVoidIf(!jSection);
		jstring jKey = FAndroidPlatformJNI::ParseFString(Env, Key);
		ReturnVoidIf(!jKey);
		(*Env).CallVoidMethod(JavaObject, Cache->EventClearMetadataValue, jSection, jKey);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}

	TSharedPtr<FJsonObject> GetMetadata(const FString& Section) override
	{
		jstring jSection = FAndroidPlatformJNI::ParseFString(Env, Section);
		ReturnNullOnFail(jSection);
		jbyteArray jMetadata = (jbyteArray)(*Env).CallStaticObjectMethod(
			Cache->BugsnagUnrealPluginClass,
			Cache->BugsnagUnrealPluginGetEventMetadataSection,
			JavaObject, jSection);
		ReturnNullOnException(Env);
		ReturnNullOnFail(jMetadata);
		return FAndroidPlatformJNI::ConvertJavaBytesToJson(Env, Cache, jMetadata);
	}

	TSharedPtr<FJsonValue> GetMetadata(const FString& Section, const FString& Key) override
	{
		jstring jSection = FAndroidPlatformJNI::ParseFString(Env, Section);
		ReturnNullOnFail(jSection);
		jstring jKey = FAndroidPlatformJNI::ParseFString(Env, Key);
		ReturnNullOnFail(jKey);
		jbyteArray jMetadata = (jbyteArray)(*Env).CallStaticObjectMethod(
			Cache->BugsnagUnrealPluginClass,
			Cache->BugsnagUnrealPluginGetEventMetadataValue,
			JavaObject, jSection, jKey);
		ReturnNullOnException(Env);
		ReturnNullOnFail(jMetadata);
		auto MetadataContainer = FAndroidPlatformJNI::ConvertJavaBytesToJson(Env, Cache, jMetadata);

		return MetadataContainer.IsValid()
				   ? MetadataContainer->TryGetField("Value")
				   : nullptr;
	}

	const TOptional<FString> GetApiKey() const override
	{
		ReturnStringFieldOptional(JavaObject, Cache->EventGetApiKey);
	}

	void SetApiKey(const TOptional<FString>& Value) override
	{
		SetStringField(Cache->EventSetApiKey, false, Value);
	}

	const TOptional<FString> GetContext() const override
	{
		ReturnStringFieldOptional(JavaObject, Cache->EventGetContext);
	}

	void SetContext(const TOptional<FString>& Value) override
	{
		SetStringField(Cache->EventSetContext, true, Value);
	}

	const TOptional<FString> GetGroupingHash() const override
	{
		ReturnStringFieldOptional(JavaObject, Cache->EventGetGroupingHash);
	}

	void SetGroupingHash(const TOptional<FString>& Value) override
	{
		SetStringField(Cache->EventSetGroupingHash, true, Value);
	}

	const TSharedRef<IBugsnagAppWithState> GetApp() const override
	{
		jobject jApp = (*Env).CallObjectMethod(JavaObject, Cache->EventGetApp);
		FAndroidPlatformJNI::CheckAndClearException(Env);
		return MakeShared<FAndroidAppWithState>(Env, Cache, jApp);
	}

	const TSharedRef<IBugsnagDeviceWithState> GetDevice() const override
	{
		jobject jDevice = (*Env).CallObjectMethod(JavaObject, Cache->EventGetDevice);
		FAndroidPlatformJNI::CheckAndClearException(Env);
		return MakeShared<FAndroidDeviceWithState>(Env, Cache, jDevice);
	}

	const TArray<TSharedRef<IBugsnagBreadcrumb>> GetBreadcrumbs() const override
	{
		return GetListItems<IBugsnagBreadcrumb, FAndroidBreadcrumb>(Cache->EventGetBreadcrumbs);
	}

	const TArray<TSharedRef<IBugsnagError>> GetErrors() const override
	{
		return GetListItems<IBugsnagError, FAndroidError>(Cache->EventGetErrors);
	}

	const TArray<TSharedRef<IBugsnagThread>> GetThreads() const override
	{
		return GetListItems<IBugsnagThread, FAndroidThread>(Cache->EventGetThreads);
	}

	const EBugsnagSeverity GetSeverity() const override
	{
		jobject jSeverity = (*Env).CallObjectMethod(JavaObject, Cache->EventGetSeverity);
		ReturnValueOnException(Env, EBugsnagSeverity::Warning);
		return FAndroidPlatformJNI::ParseSeverity(Env, Cache, jSeverity);
	}

	void SetSeverity(EBugsnagSeverity Value) override
	{
		jobject jSeverity = FAndroidPlatformJNI::ParseSeverity(Env, Cache, Value);
		SetField(Cache->EventSetSeverity, false, jSeverity);
	}

	const bool GetUnhandled() const override
	{
		jboolean jUnhandled = (*Env).CallBooleanMethod(JavaObject, Cache->EventGetUnhandled);
		FAndroidPlatformJNI::CheckAndClearException(Env);
		return jUnhandled == JNI_TRUE;
	}

	void SetUnhandled(bool Value) override
	{
		(*Env).CallVoidMethod(JavaObject, Cache->EventSetUnhandled, Value ? JNI_TRUE : JNI_FALSE);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}

	const FBugsnagUser GetUser() const override
	{
		jobject jUser = (*Env).CallObjectMethod(JavaObject, Cache->EventGetUser);
		return FAndroidPlatformJNI::ParseJavaUser(Env, Cache, jUser);
	}

	void SetUser(const TOptional<FString>& Id, const TOptional<FString>& Email, const TOptional<FString>& Name) override
	{
		jstring jId = Id.IsSet() ? FAndroidPlatformJNI::ParseFString(Env, Id.GetValue()) : nullptr;
		jstring jName = Name.IsSet() ? FAndroidPlatformJNI::ParseFString(Env, Name.GetValue()) : nullptr;
		jstring jEmail = Email.IsSet() ? FAndroidPlatformJNI::ParseFString(Env, Email.GetValue()) : nullptr;
		(*Env).CallVoidMethod(JavaObject, Cache->EventSetUser, jId, jEmail, jName);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}
};
