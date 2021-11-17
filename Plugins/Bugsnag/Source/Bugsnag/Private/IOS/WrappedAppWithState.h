// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "BugsnagAppWithState.h"
#include "WrappedApp.h"

#import <Bugsnag/BugsnagAppWithState.h>

class BUGSNAG_API FWrappedAppWithState : public IBugsnagAppWithState, FWrappedApp
{
public:
	static TSharedRef<FWrappedAppWithState> From(BugsnagAppWithState* CocoaAppWithState)
	{
		return MakeShared<FWrappedAppWithState>(CocoaAppWithState);
	}

	FWrappedAppWithState(BugsnagAppWithState* CocoaAppWithState)
		: FWrappedApp(CocoaAppWithState)
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
