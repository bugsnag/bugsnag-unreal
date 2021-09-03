#pragma once

class FBugsnagLastRunInfo
{
public:
	FBugsnagLastRunInfo(uint64 ConsecutiveLaunchCrashes, bool bCrashed, bool bCrashedDuringLaunch)
		: ConsecutiveLaunchCrashes(ConsecutiveLaunchCrashes)
		, bCrashed(bCrashed)
		, bCrashedDuringLaunch(bCrashedDuringLaunch)
	{
	}

	uint64 GetConsecutiveLaunchCrashes() const { return ConsecutiveLaunchCrashes; }

	bool GetCrashed() const { return bCrashed; }

	bool GetCrashedDuringLaunch() const { return bCrashedDuringLaunch; }

private:
	uint64 ConsecutiveLaunchCrashes;
	bool bCrashed;
	bool bCrashedDuringLaunch;
};
