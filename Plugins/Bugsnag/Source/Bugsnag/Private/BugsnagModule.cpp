// Copyright 2021 Bugsnag. All Rights Reserved.

#include "BugsnagFunctionLibrary.h"
#include "LogBugsnag.h"

#include "Modules/ModuleManager.h"

class BUGSNAG_API FBugsnagModule : public IModuleInterface
{
public:
	void StartupModule() override
	{
		const UBugsnagSettings* Settings = GetDefault<UBugsnagSettings>();
		if (!Settings)
		{
			UE_LOG(LogBugsnag, Error, TEXT("Cannot get default UBugsnagSettings object"));
			return;
		}
		if (Settings->bStartAutomaticallyAtLaunch)
		{
			StartBugsnagAutomatically();
		}
	}

	void StartBugsnagAutomatically()
	{
		TSharedPtr<FBugsnagConfiguration> Configuration = FBugsnagConfiguration::Load();
		if (!Configuration.IsValid())
		{
			UE_LOG(LogBugsnag, Error, TEXT("Cannot create FBugsnagConfiguration object"));
			return;
		}
		if (Configuration->GetApiKey().IsEmpty())
		{
			UE_LOG(LogBugsnag, Error, TEXT("Cannot start Bugsnag automatically because no ApiKey has been configured"));
			return;
		}
		UBugsnagFunctionLibrary::Start(Configuration.ToSharedRef());
	}

	bool SupportsDynamicReloading() override
	{
		return false;
	}
};

IMPLEMENT_MODULE(FBugsnagModule, Bugsnag)
