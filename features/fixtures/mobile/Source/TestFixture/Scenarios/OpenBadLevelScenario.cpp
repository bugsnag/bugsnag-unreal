#include "Scenario.h"

#include "Kismet/GameplayStatics.h"

class OpenBadLevelScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->SetEnabledBreadcrumbTypes(EBugsnagEnabledBreadcrumbTypes::Navigation);
	}

	void Run() override
	{
		// Will be processed safely on the next tick in UGameEngine::Tick()
		UGameplayStatics::OpenLevel(GetCurrentPlayWorld(), TEXT("/Game/NonExistant"));

		FCoreUObjectDelegates::PostLoadMapWithWorld.AddLambda([](UWorld* World)
			{
				if (!World)
				{
					UBugsnagFunctionLibrary::Notify(TEXT("LoadMapFailed"), TEXT("Intentionally blank"));
				}
			});
	}
};

REGISTER_SCENARIO(OpenBadLevelScenario);
