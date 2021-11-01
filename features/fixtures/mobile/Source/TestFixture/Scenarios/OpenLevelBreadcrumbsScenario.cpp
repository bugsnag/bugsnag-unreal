#include "Scenario.h"

#include "Kismet/GameplayStatics.h"

class OpenLevelBreadcrumbsScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->SetEnabledBreadcrumbTypes(EBugsnagEnabledBreadcrumbTypes::Navigation);
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
