// Copyright 2022 Bugsnag. All Rights Reserved.

#pragma once

#include "AppleBugsnagUtils.h"
#include "BugsnagFeatureFlagStore.h"

#import <Bugsnag/BugsnagFeatureFlagStore.h>

class FWrappedFeatureFlagStore : public virtual IBugsnagFeatureFlagStore
{
public:
	FWrappedFeatureFlagStore(id<BugsnagFeatureFlagStore> CocoaStore = nil)
		: CocoaStore(CocoaStore)
	{
	}

	void AddFeatureFlag(const FString& Name, const TOptional<FString>& Variant = TOptional<FString>()) override
	{
		[CocoaStore addFeatureFlagWithName:NSStringFromFString(Name) variant:NSStringFromOptional(Variant)];
	}

	void AddFeatureFlags(const TArray<FBugsnagFeatureFlag>& FeatureFlags) override
	{
		NSMutableArray* flags = [NSMutableArray array];

		for (const FBugsnagFeatureFlag& FeatureFlag : FeatureFlags)
		{
			[flags addObject:[BugsnagFeatureFlag flagWithName:NSStringFromFString(FeatureFlag.GetName())
													  variant:NSStringFromOptional(FeatureFlag.GetVariant())]];
		}

		[CocoaStore addFeatureFlags:flags];
	}

	void ClearFeatureFlag(const FString& Name) override
	{
		[CocoaStore clearFeatureFlagWithName:NSStringFromFString(Name)];
	}

	void ClearFeatureFlags() override
	{
		[CocoaStore clearFeatureFlags];
	}

	id<BugsnagFeatureFlagStore> CocoaStore;
};
