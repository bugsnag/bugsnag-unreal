#include "Scenario.h"

#include "Dom/JsonValue.h"

class NotifyWithLaunchInfoScenario : public Scenario
{
public:
	void Configure() override
	{
		// notifying from configure as a hack to get around clearing persistent
		// data between scenarios
		FTimerHandle TimerHandle;
		GetCurrentPlayWorld()->GetTimerManager().SetTimer(
			TimerHandle, []()
			{
				TSharedPtr<FBugsnagLastRunInfo> LastRunInfo = UBugsnagFunctionLibrary::GetLastRunInfo();
				if (LastRunInfo.IsValid())
				{
					UBugsnagFunctionLibrary::AddMetadata("lastRunInfo", "consecutiveLaunchCrashes", LastRunInfo->GetConsecutiveLaunchCrashes());
					UBugsnagFunctionLibrary::AddMetadata("lastRunInfo", "crashed", LastRunInfo->GetCrashed());
					UBugsnagFunctionLibrary::AddMetadata("lastRunInfo", "crashedDuringLaunch", LastRunInfo->GetCrashedDuringLaunch());
				}

				UBugsnagFunctionLibrary::Notify(TEXT("Resolution failed"), TEXT("invalid index (-1)"));
			},
			1.f, false);
	}

	void Run() override
	{
	}
};

REGISTER_SCENARIO(NotifyWithLaunchInfoScenario);
