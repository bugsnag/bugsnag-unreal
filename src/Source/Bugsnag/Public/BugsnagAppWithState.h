// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "BugsnagApp.h"

/**
 * Stateful information information about the app where the error occurred.
 *
 * Can be amended from an OnError callback if necessary.
 */
class BUGSNAG_API IBugsnagAppWithState : virtual public IBugsnagApp
{
public:
	/**
	 * The number of milliseconds the application was running before the event occurred.
	 */
	virtual TOptional<uint64> GetDuration() const = 0;

	virtual void SetDuration(TOptional<uint64>) = 0;

	/**
	 * The number of milliseconds the application was running in the foreground before the event occurred.
	 */
	virtual TOptional<uint64> GetDurationInForeground() const = 0;

	virtual void SetDurationInForeground(TOptional<uint64>) = 0;

	/**
	 * Whether the application was in the foreground when the event occurred.
	 */
	virtual TOptional<bool> GetIsInForeground() const = 0;

	virtual void SetIsInForeground(TOptional<bool>) = 0;

	/**
	 * Whether the application was still launching when the event occurred.
	 */
	virtual TOptional<bool> GetIsLaunching() const = 0;

	virtual void SetIsLaunching(TOptional<bool>) = 0;
};
