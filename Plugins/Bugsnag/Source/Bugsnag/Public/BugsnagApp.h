#pragma once

#include "CoreMinimal.h"

class BUGSNAG_API IBugsnagApp
{
public:
	// binaryArch?: string;

	virtual const TSharedPtr<FString> GetBinaryArch() const = 0;

	virtual void SetBinaryArch(const TSharedPtr<FString>&) = 0;

	// buildUuid?: string;

	virtual const TSharedPtr<FString> GetBuildUuid() const = 0;

	virtual void SetBuildUuid(const TSharedPtr<FString>&) = 0;

	// bundleVersion?: string;

	virtual const TSharedPtr<FString> GetBundleVersion() const = 0;

	virtual void SetBundleVersion(const TSharedPtr<FString>&) = 0;

	// dsymUuid?: string;

	virtual const TSharedPtr<FString> GetDsymUuid() const = 0;

	virtual void SetDsymUuid(const TSharedPtr<FString>&) = 0;

	// id?: string;

	virtual const TSharedPtr<FString> GetId() const = 0;

	virtual void SetId(const TSharedPtr<FString>&) = 0;

	// releaseStage: string = "production";

	virtual const TSharedPtr<FString> GetReleaseStage() const = 0;

	virtual void SetReleaseStage(const TSharedPtr<FString>&) = 0;

	// type?: string;

	virtual const TSharedPtr<FString> GetType() const = 0;

	virtual void SetType(const TSharedPtr<FString>&) = 0;

	// version?: string;

	virtual const TSharedPtr<FString> GetVersion() const = 0;

	virtual void SetVersion(const TSharedPtr<FString>&) = 0;

	// versionCode?: number;

	virtual const TSharedPtr<uint64> GetVersionCode() const = 0;

	virtual void SetVersionCode(const TSharedPtr<uint64>&) = 0;
};
