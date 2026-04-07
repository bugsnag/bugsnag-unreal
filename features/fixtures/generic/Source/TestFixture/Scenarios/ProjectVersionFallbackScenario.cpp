#include "Scenario.h"

#include "Misc/ConfigCacheIni.h"

class ProjectVersionFallbackScenario : public Scenario
{
public:
	void Configure() override
	{
#if PLATFORM_MAC
		if (GConfig)
		{
			GConfig->SetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), TEXT("4.5.6"), GGameIni);
		}

		delete Configuration;
		Configuration = new FBugsnagConfiguration(ApiKey);
		Configuration->SetEndpoints(NotifyEndpoint, SessionsEndpoint);
#endif
	}

	void StartBugsnag() override
	{
		Scenario::StartBugsnag();

#if PLATFORM_MAC
		if (GConfig)
		{
			GConfig->RemoveKey(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), GGameIni);
		}
#endif
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::Notify(TEXT("ProjectVersionFallback"), TEXT("Ensure app.version falls back to ProjectVersion"));
	}
};

REGISTER_SCENARIO(ProjectVersionFallbackScenario);
