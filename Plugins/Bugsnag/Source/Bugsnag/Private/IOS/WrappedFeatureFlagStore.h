// Copyright 2022 Bugsnag. All Rights Reserved.

#pragma once

#include "AppleBugsnagUtils.h"
#include "BugsnagFeatureFlagStore.h"

class FWrappedFeatureFlagStore : public virtual IBugsnagFeatureFlagStore
{
public:
	FWrappedFeatureFlagStore(id CocoaStore = nil)
		: CocoaStore(CocoaStore)
	{
	}

	void AddFeatureFlag(const FString& Name, const TOptional<FString>& Variant = TOptional<FString>()) override
	{
		// TODO
	}

	void AddFeatureFlags(const TArray<FBugsnagFeatureFlag>& FeatureFlags) override
	{
		// TODO
	}

	void ClearFeatureFlag(const FString& Name) override
	{
		// TODO
	}

	void ClearFeatureFlags() override
	{
		// TODO
	}

	id CocoaStore;
};
