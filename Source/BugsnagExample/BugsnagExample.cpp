// Copyright 2021 Bugsnag. All Rights Reserved.

#include "BugsnagExample.h"
#include "Modules/ModuleManager.h"

#include "BugsnagFunctionLibrary.h"

//
// Bugsnag will start automatically if you have configured an API key through the plug-in's settings pane (in DefaultEngine.ini)
// so by default you do not need to start it manually.
//
void FBugsnagExampleModule::StartupModule()
{
}

//
// If an API key has not been specified in DefaultEngine.ini then this is the simplest way to start Bugsnag programatically.
//
void StartWithApiKey()
{
	UBugsnagFunctionLibrary::Start(TEXT("YOUR-API-KEY"));
}

//
// Bugsnag behavior can be configured by creating an FBugsnagConfiguration object and passing it to UBugsnagFunctionLibrary::Start()
//
// All subsequent setup is optional, and will configure your Bugsnag setup in different ways.
// A few common examples are included here, for more detailed explanations please look at the documented configuration options at
// https://docs.bugsnag.com/platforms/unreal/configuration-options/
//
void StartWithConfiguration()
{
	//
	// Create an empty configuration object
	//
	TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(TEXT("YOUR-API-KEY"));

	//
	// This sets some user information that will be attached to each error.
	//
	Configuration->SetUser(FString(TEXT("DefaultUser")), FString(TEXT("user@example.com")), FString(TEXT("Default User")));

	//
	// When persisting a user you won't need to set the user information every time the app opens, it will be persisted between app sessions.
	//
	Configuration->SetPersistUser(true);

	//
	// The appVersion will let you see what release an error is present in.
	// This will be picked up automatically from your app at run-time, but can be manually overriden here if necessary.
	//
	Configuration->SetAppVersion(FString(TEXT("1.5.0")));

	//
	// Enabled error types allow you to customize exactly which errors are automatically captured and delivered to your Bugsnag dashboard.
	// A detailed breakdown of each error type can be found in the configuration option documentation.
	//
	FBugsnagErrorTypes EnabledErrorTypes;
	EnabledErrorTypes.bOOMs = false;
	EnabledErrorTypes.bNativeCrashes = true;
	EnabledErrorTypes.bUnhandledExceptions = true;
	Configuration->SetEnabledErrorTypes(EnabledErrorTypes);

	//
	// If there's information that you do not wish sent to your Bugsnag dashboard, such as passwords or user information, you can set the keys as
	// redacted. When a notification is sent to Bugsnag all keys matching your set filters will be redacted before they leave your application.
	// All automatically captured data can be found here: https://docs.bugsnag.com/platforms/unreal/automatically-captured-data/
	//
	Configuration->SetRedactedKeys({TEXT("filter_me"), TEXT("firstName"), TEXT("lastName")});

	//
	// Start Bugsnag with the specified configuration
	//
	UBugsnagFunctionLibrary::Start(Configuration);

	//
	// If the last run crashed during its launch, you may want to take special actions such as clearing caches or not attempting to restore state.
	//
	TSharedPtr<FBugsnagLastRunInfo> LastRunInfo = UBugsnagFunctionLibrary::GetLastRunInfo();
	if (LastRunInfo.IsValid() && LastRunInfo->GetCrashedDuringLaunch())
	{
		// Do something to prevent another crash during launch
	}
}

void FBugsnagExampleModule::ShutdownModule()
{
	UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("FBugsnagExampleModule shutting down"));
}

IMPLEMENT_PRIMARY_GAME_MODULE(FBugsnagExampleModule, BugsnagExample, "BugsnagExample");
