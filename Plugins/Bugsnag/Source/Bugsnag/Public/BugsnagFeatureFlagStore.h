// Copyright 2022 Bugsnag. All Rights Reserved.

#pragma once

#include "BugsnagFeatureFlag.h"

class BUGSNAG_API IBugsnagFeatureFlagStore
{
public:
	/**
	 * Add a single feature flag with optional experiment membership/variant.
	 * Any existing flag with the same name will be replaced with the newer value.
	 *
	 * @param Name    The name of the feature flag to add.
	 * @param Variant The variant to set the feature flag to.
	 */
	virtual void AddFeatureFlag(const FString& Name, const TOptional<FString>& Variant = TOptional<FString>()) = 0;

	/**
	 * Add multiple feature flags and any associated experiment membership.
	 * Existing flags with matching names will be replaced with the newer values.
	 *
	 * @param FeatureFlags An array of feature flag objects to add.
	 */
	virtual void AddFeatureFlags(const TArray<FBugsnagFeatureFlag>& FeatureFlags) = 0;

	/**
	 * Remove a single feature flag and any associated experiment membership.
	 *
	 * @param Name The name of the feature flag to remove.
	 */
	virtual void ClearFeatureFlag(const FString& Name) = 0;

	/**
	 * Remove all feature flag and experiment membership data.
	 */
	virtual void ClearFeatureFlags() = 0;
};
