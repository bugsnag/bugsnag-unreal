#include "Scenario.h"

#include "Kismet/GameplayStatics.h"

//
// Opens a new level to verify that custom context is not overwritten
//
class CustomContextOpenLevelScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->SetContext(FString(TEXT("game")));
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
				UBugsnagFunctionLibrary::Notify(TEXT(""), TEXT(""));
			},
			2.f, false);
	}
};

REGISTER_SCENARIO(CustomContextOpenLevelScenario);
