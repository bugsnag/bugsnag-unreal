// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "AndroidApp.h"
#include "BugsnagAppWithState.h"

class FAndroidAppWithState : public IBugsnagAppWithState, FAndroidApp
{
public:
	using FAndroidApp::FAndroidApp;

	TOptional<uint64> GetDuration() const override
	{
		return GetLongObjectField<uint64>(Cache->AppWithStateGetDuration);
	}

	void SetDuration(TOptional<uint64> Value) override
	{
		SetLongObjectField(Cache->AppWithStateSetDuration, true, Value);
	}

	TOptional<uint64> GetDurationInForeground() const override
	{
		return GetLongObjectField<uint64>(Cache->AppWithStateGetDurationInForeground);
	}

	void SetDurationInForeground(TOptional<uint64> Value) override
	{
		SetLongObjectField<uint64>(Cache->AppWithStateSetDurationInForeground, true, Value);
	}

	TOptional<bool> GetIsInForeground() const override
	{
		return GetBoolObjectField(Cache->AppWithStateGetInForeground);
	}

	void SetIsInForeground(TOptional<bool> Value) override
	{
		SetBoolObjectField(Cache->AppWithStateSetInForeground, true, Value);
	}

	TOptional<bool> GetIsLaunching() const override
	{
		return GetBoolObjectField(Cache->AppWithStateGetIsLaunching);
	}

	void SetIsLaunching(TOptional<bool> Value) override
	{
		SetBoolObjectField(Cache->AppWithStateSetIsLaunching, true, Value);
	}
};
