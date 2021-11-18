// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class BUGSNAG_API IBugsnagApp
{
public:
	// binaryArch?: string;

	virtual const TOptional<FString> GetBinaryArch() const = 0;

	virtual void SetBinaryArch(const TOptional<FString>&) = 0;

	// buildUuid?: string;

	virtual const TOptional<FString> GetBuildUuid() const = 0;

	virtual void SetBuildUuid(const TOptional<FString>&) = 0;

	// bundleVersion?: string;

	virtual const TOptional<FString> GetBundleVersion() const = 0;

	virtual void SetBundleVersion(const TOptional<FString>&) = 0;

	// dsymUuid?: string;

	virtual const TOptional<FString> GetDsymUuid() const = 0;

	virtual void SetDsymUuid(const TOptional<FString>&) = 0;

	// id?: string;

	virtual const TOptional<FString> GetId() const = 0;

	virtual void SetId(const TOptional<FString>&) = 0;

	// releaseStage: string = "production";

	virtual const TOptional<FString> GetReleaseStage() const = 0;

	virtual void SetReleaseStage(const TOptional<FString>&) = 0;

	// type?: string;

	virtual const TOptional<FString> GetType() const = 0;

	virtual void SetType(const TOptional<FString>&) = 0;

	// version?: string;

	virtual const TOptional<FString> GetVersion() const = 0;

	virtual void SetVersion(const TOptional<FString>&) = 0;

	// versionCode?: number;

	virtual const TOptional<int32> GetVersionCode() const = 0;

	virtual void SetVersionCode(const TOptional<int32>&) = 0;
};
