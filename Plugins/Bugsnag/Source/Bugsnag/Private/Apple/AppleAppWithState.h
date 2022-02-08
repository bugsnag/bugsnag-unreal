// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "AppleApp.h"
#include "BugsnagAppWithState.h"

#import <Bugsnag/BugsnagAppWithState.h>

class BUGSNAG_API FAppleAppWithState : public IBugsnagAppWithState, FAppleApp
{
public:
	static TSharedRef<FAppleAppWithState> From(BugsnagAppWithState* CocoaAppWithState)
	{
		return MakeShared<FAppleAppWithState>(CocoaAppWithState);
	}

	FAppleAppWithState(BugsnagAppWithState* CocoaAppWithState)
		: FAppleApp(CocoaAppWithState)
		, CocoaAppWithState(CocoaAppWithState)
	{
	}

	// duration?: number;

	TOptional<uint64> GetDuration() const override
	{
		return OptionalUInt64FromNSNumber(CocoaAppWithState.duration);
	}

	void SetDuration(TOptional<uint64> Value) override
	{
		CocoaAppWithState.duration = NSNumberFromOptional(Value);
	}

	// durationInForeground?: number;

	TOptional<uint64> GetDurationInForeground() const override
	{
		return OptionalUInt64FromNSNumber(CocoaAppWithState.durationInForeground);
	}

	void SetDurationInForeground(TOptional<uint64> Value) override
	{
		CocoaAppWithState.durationInForeground = NSNumberFromOptional(Value);
	}

	// inForeground?: boolean;

	TOptional<bool> GetIsInForeground() const override
	{
		return CocoaAppWithState.inForeground;
	}

	void SetIsInForeground(TOptional<bool> Value) override
	{
		CocoaAppWithState.inForeground = Value.Get(false);
	}

	// isLaunching?: boolean;

	TOptional<bool> GetIsLaunching() const override
	{
		return CocoaAppWithState.isLaunching;
	}

	void SetIsLaunching(TOptional<bool> Value) override
	{
		CocoaAppWithState.isLaunching = Value.Get(false);
	}

private:
	BugsnagAppWithState* CocoaAppWithState;
};
