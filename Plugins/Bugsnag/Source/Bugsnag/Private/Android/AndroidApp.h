#pragma once

#include "BugsnagApp.h"
#include "JavaObjectWrapper.h"

class FAndroidApp : virtual public IBugsnagApp, public FJavaObjectWrapper
{
public:
	using FJavaObjectWrapper::FJavaObjectWrapper;

	const TOptional<FString> GetBinaryArch() const
	{
		ReturnStringFieldOptional(JavaObject, Cache->AppGetBinaryArch);
	}

	void SetBinaryArch(const TOptional<FString>& Value)
	{
		SetStringField(Cache->AppSetBinaryArch, true, Value);
	}

	const TOptional<FString> GetBuildUuid() const
	{
		ReturnStringFieldOptional(JavaObject, Cache->AppGetBuildUuid);
	}

	void SetBuildUuid(const TOptional<FString>& Value)
	{
		SetStringField(Cache->AppSetBuildUuid, true, Value);
	}

	const TOptional<FString> GetId() const
	{
		ReturnStringFieldOptional(JavaObject, Cache->AppGetId);
	}

	void SetId(const TOptional<FString>& Value)
	{
		SetStringField(Cache->AppSetId, true, Value);
	}

	const TOptional<FString> GetReleaseStage() const
	{
		ReturnStringFieldOptional(JavaObject, Cache->AppGetReleaseStage);
	}

	void SetReleaseStage(const TOptional<FString>& Value)
	{
		SetStringField(Cache->AppSetReleaseStage, true, Value);
	}

	const TOptional<FString> GetType() const
	{
		ReturnStringFieldOptional(JavaObject, Cache->AppGetType);
	}

	void SetType(const TOptional<FString>& Value)
	{
		SetStringField(Cache->AppSetType, true, Value);
	}

	const TOptional<FString> GetVersion() const
	{
		ReturnStringFieldOptional(JavaObject, Cache->AppGetVersion);
	}

	void SetVersion(const TOptional<FString>& Value)
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

	const TOptional<FString> GetBundleVersion() const
	{
		return TOptional<FString>();
	}

	void SetBundleVersion(const TOptional<FString>& Value)
	{
	}

	const TOptional<FString> GetDsymUuid() const
	{
		return TOptional<FString>();
	}

	void SetDsymUuid(const TOptional<FString>& Value)
	{
	}
};
