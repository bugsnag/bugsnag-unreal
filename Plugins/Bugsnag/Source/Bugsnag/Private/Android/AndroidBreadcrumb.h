#pragma once

#include "BugsnagBreadcrumb.h"
#include "JavaObjectWrapper.h"

class FAndroidBreadcrumb : public IBugsnagBreadcrumb, FJavaObjectWrapper
{
public:
	using FJavaObjectWrapper::FJavaObjectWrapper;

	const FDateTime GetTimestamp() const override
	{
		return GetDateField(Cache->BreadcrumbGetTimestamp);
	}

	const FString GetMessage() const override
	{
		return GetStringField(Cache->BreadcrumbGetMessage);
	}

	void SetMessage(const FString& Message) override
	{
		SetStringField(Cache->BreadcrumbSetMessage, false, MakeShareable(new FString(Message)));
	}

	EBugsnagBreadcrumbType GetType() const override
	{
		jobject jType = (*Env).CallObjectMethod(JavaObject, Cache->BreadcrumbGetType);
		ReturnValueOnException(Env, EBugsnagBreadcrumbType::Manual);
		const char* Name = FAndroidPlatformJNI::GetNameFromEnum(Env, Cache, jType);
		if (!Name)
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
		SetField(Cache->BreadcrumbSetType, false, FAndroidPlatformJNI::ParseBreadcrumbType(Env, Cache, Type));
	}

	TSharedPtr<FJsonObject> GetMetadata() const override
	{
		return GetJsonObjectField(Cache->BreadcrumbGetMetadata);
	}

	void SetMetadata(const TSharedPtr<FJsonObject>& Metadata) override
	{
		SetField(Cache->BreadcrumbSetMetadata, false, FAndroidPlatformJNI::ParseJsonObject(Env, Cache, Metadata));
	}
};
