// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "../LogTestFixture.h"
#include "BugsnagFunctionLibrary.h"

#include "Dom/JsonObject.h"

#include <map>
#include <string>

class Scenario
{
public:
	static Scenario* CurrentScenario;

	static void ClearPersistentData();

	static void Start(const FString& ScenarioName)
	{
		UE_LOG(LogTestFixture, Display, TEXT("Starting scenario \"%s\""), *ScenarioName);

		if (!Instantiate(TCHAR_TO_UTF8(*ScenarioName)))
		{
			UE_LOG(LogTestFixture, Error, TEXT("Could not instantiate scenario \"%s\""), *ScenarioName);
			return;
		}

		UE_LOG(LogTestFixture, Display, TEXT("%s::Configure()"), *ScenarioName);
		CurrentScenario->Configure();

		UE_LOG(LogTestFixture, Display, TEXT("%s::StartBugsnag()"), *ScenarioName);
		CurrentScenario->StartBugsnag();
	}

	static void Run(const FString& ScenarioName)
	{
		ClearPersistentData();

		Start(ScenarioName);

		if (CurrentScenario)
		{
			UE_LOG(LogTestFixture, Display, TEXT("%s::Run()"), *ScenarioName);

			CurrentScenario->Run();
		}
	}

	/**
	 * Grabs the current world reference (replaces same-named helper on
	 * UEngine which is unavailable in UE 4.23 and earlier)
	 */
	static UWorld* GetCurrentPlayWorld();

	////////////////////////////////////////////////////////////////////////////

	const FString ApiKey = TEXT("12312312312312312312312312312312");

#if PLATFORM_ANDROID || PLATFORM_IOS
	const FString NotifyEndpoint = TEXT("http://bs-local.com:9339/notify");
	const FString SessionsEndpoint = TEXT("http://bs-local.com:9339/sessions");
#else
	const FString NotifyEndpoint = TEXT("http://localhost:9339/notify");
	const FString SessionsEndpoint = TEXT("http://localhost:9339/sessions");
#endif

	Scenario()
	{
		Configuration = new FBugsnagConfiguration(ApiKey);
		Configuration->SetEndpoints(NotifyEndpoint, SessionsEndpoint);
	}

	virtual void Configure()
	{
	}

	virtual void StartBugsnag()
	{
		UBugsnagFunctionLibrary::Start(MakeShareable(Configuration));
	}

	virtual void Run() = 0;

	FBugsnagConfiguration* Configuration;

	////////////////////////////////////////////////////////////////////////////

	typedef class Scenario* (*Factory)(void);

	static int Register(const std::string& Name, Factory Factory)
	{
		GetFactories()[Name] = Factory;
		return 1;
	}

	static bool Instantiate(const std::string& Name)
	{
		Factory Factory = GetFactories()[Name];
		if (!Factory)
		{
			// avoid crash if invoking on null
			return false;
		}
		CurrentScenario = Factory();
		return CurrentScenario != nullptr;
	}

private:
	static std::map<std::string, Factory>& GetFactories()
	{
		static std::map<std::string, Factory> Factories;
		return Factories;
	}
};

#define REGISTER_SCENARIO(CLASSNAME)                                      \
	static Scenario* Instantiate___##CLASSNAME(void)                      \
	{                                                                     \
		return new CLASSNAME;                                             \
	}                                                                     \
	static int Register___##CLASSNAME()                                   \
	{                                                                     \
		return Scenario::Register(#CLASSNAME, Instantiate___##CLASSNAME); \
	}                                                                     \
	static int Registered___##CLASSNAME = Register___##CLASSNAME();
