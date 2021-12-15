#include "Scenario.h"
#include <cstdlib>

class CrashAfterLaunchedScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->AddOnSendError([](TSharedRef<IBugsnagEvent> Event)
			{
				TSharedPtr<FBugsnagLastRunInfo> LastRunInfo = UBugsnagFunctionLibrary::GetLastRunInfo();
				if (LastRunInfo.IsValid())
				{
					Event->AddMetadata(TEXT("lastRunInfo"), TEXT("consecutiveLaunchCrashes"), LastRunInfo->GetConsecutiveLaunchCrashes());
					Event->AddMetadata(TEXT("lastRunInfo"), TEXT("crashed"), LastRunInfo->GetCrashed());
					Event->AddMetadata(TEXT("lastRunInfo"), TEXT("crashedDuringLaunch"), LastRunInfo->GetCrashedDuringLaunch());
				}

				return true;
			});
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::MarkLaunchCompleted();
		abort();
	}
};

REGISTER_SCENARIO(CrashAfterLaunchedScenario);
