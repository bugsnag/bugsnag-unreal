#pragma once

#include "BugsnagApp.h"
#include "JavaObjectWrapper.h"

class FAndroidApp : virtual public IBugsnagApp, public FJavaObjectWrapper
{
public:
	using FJavaObjectWrapper::FJavaObjectWrapper;

	const TSharedPtr<FString> GetBinaryArch() const
	{
		ReturnStringFieldPtr(JavaObject, Cache->AppGetBinaryArch);
	}

	void SetBinaryArch(const TSharedPtr<FString>& Value)
	{
		SetStringField(Cache->AppSetBinaryArch, true, Value);
	}

	const TSharedPtr<FString> GetBuildUuid() const
	{
		ReturnStringFieldPtr(JavaObject, Cache->AppGetBuildUuid);
	}

	void SetBuildUuid(const TSharedPtr<FString>& Value)
	{
		SetStringField(Cache->AppSetBuildUuid, true, Value);
	}

	const TSharedPtr<FString> GetId() const
	{
		ReturnStringFieldPtr(JavaObject, Cache->AppGetId);
	}

	void SetId(const TSharedPtr<FString>& Value)
	{
		SetStringField(Cache->AppSetId, true, Value);
	}

	const TSharedPtr<FString> GetReleaseStage() const
	{
		ReturnStringFieldPtr(JavaObject, Cache->AppGetReleaseStage);
	}

	void SetReleaseStage(const TSharedPtr<FString>& Value)
	{
		SetStringField(Cache->AppSetReleaseStage, true, Value);
	}

	const TSharedPtr<FString> GetType() const
	{
		ReturnStringFieldPtr(JavaObject, Cache->AppGetType);
	}

	void SetType(const TSharedPtr<FString>& Value)
	{
		SetStringField(Cache->AppSetType, true, Value);
	}

	const TSharedPtr<FString> GetVersion() const
	{
		ReturnStringFieldPtr(JavaObject, Cache->AppGetVersion);
	}

	void SetVersion(const TSharedPtr<FString>& Value)
	{
		SetStringField(Cache->AppSetVersion, true, Value);
	}

	const TOptional<int64> GetVersionCode() const
	{
		return GetLongObjectField<int64>(Cache->AppGetVersionCode);
	}

	void SetVersionCode(const TOptional<int64>& Value)
	{
		SetLongObjectField(Cache->AppSetVersionCode, true, Value);
	}

	// -- unsupported fields

	const TSharedPtr<FString> GetBundleVersion() const
	{
		return nullptr;
	}

	void SetBundleVersion(const TSharedPtr<FString>& Value)
	{
	}

	const TSharedPtr<FString> GetDsymUuid() const
	{
		return nullptr;
	}

	void SetDsymUuid(const TSharedPtr<FString>& Value)
	{
	}
};
