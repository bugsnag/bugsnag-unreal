// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Stateless information about the app where the error occurred.
 *
 * Can be amended from an OnError or OnSession callback if necessary.
 */
class BUGSNAG_API IBugsnagApp
{
public:
	/**
	 * The architecture of the running application binary.
	 */
	virtual const TOptional<FString> GetBinaryArch() const = 0;

	virtual void SetBinaryArch(const TOptional<FString>&) = 0;

	/**
	 * The unique identifier for the build of the application (Android only.)
	 */
	virtual const TOptional<FString> GetBuildUuid() const = 0;

	virtual void SetBuildUuid(const TOptional<FString>&) = 0;

	/**
	 * The bundle version used by the application (iOS & macOS only.)
	 */
	virtual const TOptional<FString> GetBundleVersion() const = 0;

	virtual void SetBundleVersion(const TOptional<FString>&) = 0;

	/**
	 * The UUID of the debug symbols file corresponding to this application, if any (iOS & macOS only.)
	 */
	virtual const TOptional<FString> GetDsymUuid() const = 0;

	virtual void SetDsymUuid(const TOptional<FString>&) = 0;

	/**
	 * The package name of the application.
	 */
	virtual const TOptional<FString> GetId() const = 0;

	virtual void SetId(const TOptional<FString>&) = 0;

	/**
	 * The release stage set in Configuration.
	 */
	virtual const TOptional<FString> GetReleaseStage() const = 0;

	virtual void SetReleaseStage(const TOptional<FString>&) = 0;

	/**
	 * The application type set in Configuration.
	 */
	virtual const TOptional<FString> GetType() const = 0;

	virtual void SetType(const TOptional<FString>&) = 0;

	/**
	 * The version of the application set in Configuration.
	 */
	virtual const TOptional<FString> GetVersion() const = 0;

	virtual void SetVersion(const TOptional<FString>&) = 0;

	/**
	 * The version code of the application set in Configuration (Android only.)
	 */
	virtual const TOptional<int32> GetVersionCode() const = 0;

	virtual void SetVersionCode(const TOptional<int32>&) = 0;
};
