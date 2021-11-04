#include "Scenario.h"

Scenario* Scenario::CurrentScenario = nullptr;

UWorld* Scenario::GetCurrentPlayWorld()
{
	if (GEngine)
	{
		for (const FWorldContext& Context : GEngine->GetWorldContexts())
		{
			if (Context.WorldType == EWorldType::Game)
			{
				return Context.World();
			}
		}
	}
	return nullptr;
}
