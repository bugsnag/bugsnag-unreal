#include "Modules/ModuleManager.h"

#include "Scenarios/Scenario.h"

class TESTFIXTURE_API FTestFixtureModule : public IModuleInterface
{
public:
	void StartupModule() override
	{
		FCoreUObjectDelegates::PostLoadMapWithWorld.AddRaw(this, &FTestFixtureModule::OnMapLoaded);
	}

	void OnMapLoaded(UWorld* World)
	{
		FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);

#if PLATFORM_MAC && !UE_EDITOR
		// Wait for map loading to prevent unexpected breadcrumbs when running scenarios.
		EvaluateCommandLine();
#endif
	}

	void EvaluateCommandLine()
	{
		const TCHAR* CommandLine = FCommandLine::GetOriginal();
		UE_LOG(LogTestFixture, Display, TEXT("Command line = %s"), CommandLine);

		FString Action;
		if (!FParse::Value(CommandLine, TEXT("action"), Action))
		{
			UE_LOG(LogTestFixture, Error, TEXT("No Action passed on command line"));
			return;
		}

		FString ScenarioName;
		if (!FParse::Value(CommandLine, TEXT("scenario_name"), ScenarioName))
		{
			UE_LOG(LogTestFixture, Error, TEXT("No ScenarioName passed on command line"));
			return;
		}

		UE_LOG(LogTestFixture, Display, TEXT("Action = \"%s\" ScenarioName = \"%s\""), *Action, *ScenarioName);

		if (Action == TEXT("run_scenario"))
		{
			Scenario::Run(ScenarioName);
		}
		else if (Action == TEXT("start_bugsnag"))
		{
			Scenario::Start(ScenarioName);
		}
		else
		{
			UE_LOG(LogTestFixture, Error, TEXT("Unrecognized action: \"%s\""), *Action);
		}
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FTestFixtureModule, TestFixture, "TestFixture");
