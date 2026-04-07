#include "Scenario.h"

#include "Misc/ConfigCacheIni.h"

class ProjectVersionFallbackScenario : public Scenario
{
public:
#if PLATFORM_MAC
	FString PriorProjectVersion;
	bool bHadPriorProjectVersion = false;
	bool bOverrodeProjectVersion = false;
#endif

	void Configure() override
	{
#if PLATFORM_MAC
		if (GConfig)
		{
			bHadPriorProjectVersion = GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), PriorProjectVersion, GGameIni);
			GConfig->SetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), TEXT("4.5.6"), GGameIni);
			bOverrodeProjectVersion = true;
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
		if (bOverrodeProjectVersion && GConfig)
		{
			if (bHadPriorProjectVersion)
			{
				GConfig->SetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), *PriorProjectVersion, GGameIni);
			}
			else
			{
				GConfig->RemoveKey(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), GGameIni);
			}
		}
#endif
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::Notify(TEXT("ProjectVersionFallback"), TEXT("Ensure app.version falls back to ProjectVersion"));
	}
};

REGISTER_SCENARIO(ProjectVersionFallbackScenario);
