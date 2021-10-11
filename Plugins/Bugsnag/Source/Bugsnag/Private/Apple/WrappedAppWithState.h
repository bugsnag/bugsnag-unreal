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

	TSharedPtr<uint64> GetDuration() const override
	{
		return UInt64PtrFromNSNumber(CocoaAppWithState.duration);
	}

	void SetDuration(TSharedPtr<uint64> Value) override
	{
		CocoaAppWithState.duration = Value.IsValid() ? @(*Value) : nil;
	}

	// durationInForeground?: number;

	TSharedPtr<uint64> GetDurationInForeground() const override
	{
		return UInt64PtrFromNSNumber(CocoaAppWithState.durationInForeground);
	}

	void SetDurationInForeground(TSharedPtr<uint64> Value) override
	{
		CocoaAppWithState.durationInForeground = Value.IsValid() ? @(*Value) : nil;
	}

	// inForeground?: boolean;

	TSharedPtr<bool> GetIsInForeground() const override
	{
		return MakeShareable(new bool(CocoaAppWithState.inForeground));
	}

	void SetIsInForeground(TSharedPtr<bool> Value) override
	{
		CocoaAppWithState.inForeground = Value.IsValid() && *Value;
	}

	// isLaunching?: boolean;

	TSharedPtr<bool> GetIsLaunching() const override
	{
		return MakeShareable(new bool(CocoaAppWithState.isLaunching));
	}

	void SetIsLaunching(TSharedPtr<bool> Value) override
	{
		CocoaAppWithState.isLaunching = Value.IsValid() && *Value;
	}

private:
	BugsnagAppWithState* CocoaAppWithState;
};
