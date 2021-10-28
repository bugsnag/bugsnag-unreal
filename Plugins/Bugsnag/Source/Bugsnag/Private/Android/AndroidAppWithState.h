#pragma once

#include "AndroidApp.h"
#include "BugsnagAppWithState.h"

class FAndroidAppWithState : public IBugsnagAppWithState, FAndroidApp
{
public:
	using FAndroidApp::FAndroidApp;

	TSharedPtr<uint64> GetDuration() const override
	{
		return GetLongObjectField<uint64>(Cache->AppWithStateGetDuration);
	}

	void SetDuration(TSharedPtr<uint64> Value) override
	{
		SetLongObjectField(Cache->AppWithStateSetDuration, true, Value);
	}

	TSharedPtr<uint64> GetDurationInForeground() const override
	{
		return GetLongObjectField<uint64>(Cache->AppWithStateGetDurationInForeground);
	}

	void SetDurationInForeground(TSharedPtr<uint64> Value) override
	{
		SetLongObjectField<uint64>(Cache->AppWithStateSetDurationInForeground, true, Value);
	}

	TSharedPtr<bool> GetIsInForeground() const override
	{
		return GetBoolObjectField(Cache->AppWithStateGetInForeground);
	}

	void SetIsInForeground(TSharedPtr<bool> Value) override
	{
		SetBoolObjectField(Cache->AppWithStateSetInForeground, true, Value);
	}

	TSharedPtr<bool> GetIsLaunching() const override
	{
		return GetBoolObjectField(Cache->AppWithStateGetIsLaunching);
	}

	void SetIsLaunching(TSharedPtr<bool> Value) override
	{
		SetBoolObjectField(Cache->AppWithStateSetIsLaunching, true, Value);
	}
};
