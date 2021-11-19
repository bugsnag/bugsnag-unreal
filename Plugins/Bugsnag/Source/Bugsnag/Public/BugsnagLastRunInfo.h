// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

/**
 * Information about the last run of the app.
 */
class FBugsnagLastRunInfo
{
public:
	FBugsnagLastRunInfo(uint64 ConsecutiveLaunchCrashes, bool bCrashed, bool bCrashedDuringLaunch)
		: ConsecutiveLaunchCrashes(ConsecutiveLaunchCrashes)
		, bCrashed(bCrashed)
		, bCrashedDuringLaunch(bCrashedDuringLaunch)
	{
	}

	/**
	 * The number of consecutive runs that have ended with a crash while launching.
	 */
	uint64 GetConsecutiveLaunchCrashes() const { return ConsecutiveLaunchCrashes; }

	/**
	 * True if the previous run crashed.
	 */
	bool GetCrashed() const { return bCrashed; }

	/**
	 * True if the previous run crashed while launching.
	 */
	bool GetCrashedDuringLaunch() const { return bCrashedDuringLaunch; }

private:
	uint64 ConsecutiveLaunchCrashes;
	bool bCrashed;
	bool bCrashedDuringLaunch;
};
