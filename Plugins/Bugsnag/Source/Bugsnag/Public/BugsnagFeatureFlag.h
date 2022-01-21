// Copyright 2022 Bugsnag. All Rights Reserved.

#pragma once

/**
 * Represents a feature flag to be added to Bugsnag.
 */
class BUGSNAG_API FBugsnagFeatureFlag
{
public:
	FBugsnagFeatureFlag(const FString& Name, const TOptional<FString>& Variant = TOptional<FString>())
		: Name(Name)
		, Variant(Variant)
	{
	}

	const FString& GetName() const { return Name; }

	const TOptional<FString> GetVariant() const { return Variant; }

private:
	FString Name;
	TOptional<FString> Variant;
};
