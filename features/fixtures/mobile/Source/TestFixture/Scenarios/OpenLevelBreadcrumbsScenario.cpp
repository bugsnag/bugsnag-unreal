#include "Scenario.h"

#include "Kismet/GameplayStatics.h"

class OpenLevelBreadcrumbsScenario : public Scenario
{
public:
	void Configure() override
	{
		FBugsnagEnabledBreadcrumbTypes BugsnagEnabledBreadcrumbTypes;
		BugsnagEnabledBreadcrumbTypes.bError = false;
		BugsnagEnabledBreadcrumbTypes.bLog = false;
		BugsnagEnabledBreadcrumbTypes.bManual = false;
		BugsnagEnabledBreadcrumbTypes.bNavigation = true;
		BugsnagEnabledBreadcrumbTypes.bProcess = false;
		BugsnagEnabledBreadcrumbTypes.bRequest = false;
		BugsnagEnabledBreadcrumbTypes.bState = false;
		BugsnagEnabledBreadcrumbTypes.bUser = false;
		Configuration->SetEnabledBreadcrumbTypes(BugsnagEnabledBreadcrumbTypes);
	}

	void Run() override
	{
		UWorld* World = GetCurrentPlayWorld();

		// Will be processed safely on the next tick in UGameEngine::Tick()
		UGameplayStatics::OpenLevel(World, TEXT("/Game/AnotherWorld"));

		// Using timer to allow OpenLevel() to complete before calling Notify()
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(
			TimerHandle, []()
			{
				UBugsnagFunctionLibrary::Notify(TEXT("OpenLevelBreadcrumbsScenario"), TEXT("Intentionally blank"));
			},
			2.f, false);
	}
};

REGISTER_SCENARIO(OpenLevelBreadcrumbsScenario);
