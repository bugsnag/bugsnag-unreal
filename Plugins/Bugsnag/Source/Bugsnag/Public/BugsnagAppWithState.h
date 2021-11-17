// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "BugsnagApp.h"

class BUGSNAG_API IBugsnagAppWithState : virtual public IBugsnagApp
{
public:
	// duration?: number;

	virtual TOptional<uint64> GetDuration() const = 0;

	virtual void SetDuration(TOptional<uint64>) = 0;

	// durationInForeground?: number;

	virtual TOptional<uint64> GetDurationInForeground() const = 0;

	virtual void SetDurationInForeground(TOptional<uint64>) = 0;

	// inForeground?: boolean;

	virtual TOptional<bool> GetIsInForeground() const = 0;

	virtual void SetIsInForeground(TOptional<bool>) = 0;

	// isLaunching?: boolean;

	virtual TOptional<bool> GetIsLaunching() const = 0;

	virtual void SetIsLaunching(TOptional<bool>) = 0;
};
