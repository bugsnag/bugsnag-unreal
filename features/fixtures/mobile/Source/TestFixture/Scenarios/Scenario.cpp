#include "Scenario.h"

Scenario* Scenario::CurrentScenario = nullptr;

UWorld* Scenario::GetCurrentPlayWorld()
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
