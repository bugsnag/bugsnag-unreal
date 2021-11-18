// Copyright 2021 Bugsnag. All Rights Reserved.

#include "BugsnagModule.h"
#include "BugsnagFunctionLibrary.h"

#include "Modules/ModuleManager.h"

void FBugsnagModule::StartupModule()
{
	const UBugsnagSettings* Settings = GetDefault<UBugsnagSettings>();
	if (!Settings || !Settings->bStartAutomaticallyAtLaunch)
	{
		return;
	}
	TSharedPtr<FBugsnagConfiguration> Configuration = FBugsnagConfiguration::Load();
	if (Configuration.IsValid() && !Configuration->GetApiKey().IsEmpty())
	{
		UBugsnagFunctionLibrary::Start(Configuration.ToSharedRef());
	}
	else
	{
		UE_LOG(LogBugsnag, Error, TEXT("Cannot start Bugsnag - ApiKey is not specified in DefaultEngine.ini"));
	}
}

void FBugsnagModule::ShutdownModule() {}

IMPLEMENT_MODULE(FBugsnagModule, Bugsnag)
