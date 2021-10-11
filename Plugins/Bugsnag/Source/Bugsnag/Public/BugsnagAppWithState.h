#pragma once

#include "BugsnagApp.h"

class BUGSNAG_API IBugsnagAppWithState : virtual public IBugsnagApp
{
public:
	// duration?: number;

	virtual TSharedPtr<uint64> GetDuration() const = 0;

	virtual void SetDuration(TSharedPtr<uint64>) = 0;

	// durationInForeground?: number;

	virtual TSharedPtr<uint64> GetDurationInForeground() const = 0;

	virtual void SetDurationInForeground(TSharedPtr<uint64>) = 0;

	// inForeground?: boolean;

	virtual TSharedPtr<bool> GetIsInForeground() const = 0;

	virtual void SetIsInForeground(TSharedPtr<bool>) = 0;

	// isLaunching?: boolean;

	virtual TSharedPtr<bool> GetIsLaunching() const = 0;

	virtual void SetIsLaunching(TSharedPtr<bool>) = 0;
};
