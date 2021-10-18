#include "Scenario.h"

#include "Kismet/GameplayStatics.h"

// Because UEngine::GetCurrentPlayWorld() does not exist in 4.23 :-/
static UWorld* GetCurrentPlayWorld()
{
	// This logic copied from FEngineService::SendPong() in Runtime/Engine/Private/EngineService.cpp
	FWorldContext const* ContextToUse = nullptr;
	if (GEngine)
	{
		// We're going to look through the WorldContexts and pull any Game context we find
		// If there isn't a Game context, we'll take the first PIE we find
		// and if none of those we'll use an Editor
		for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
		{
			if (WorldContext.WorldType == EWorldType::Game)
			{
				ContextToUse = &WorldContext;
				break;
			}
			else if (WorldContext.WorldType == EWorldType::PIE && (ContextToUse == nullptr || ContextToUse->WorldType != EWorldType::PIE))
			{
				ContextToUse = &WorldContext;
			}
			else if (WorldContext.WorldType == EWorldType::Editor && ContextToUse == nullptr)
			{
				ContextToUse = &WorldContext;
			}
		}
	}
	return ContextToUse ? ContextToUse->World() : nullptr;
}

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
