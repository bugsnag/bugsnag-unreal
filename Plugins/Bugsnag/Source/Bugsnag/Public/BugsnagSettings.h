// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "BugsnagSettings.generated.h"

USTRUCT()
struct FBugsnagEnabledBreadcrumbTypes
{
	GENERATED_BODY()

	// An error detected by Bugsnag or reported via Notify()
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	bool bError = true;

	// A log message
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	bool bLog = true;

	// A breadcrumb left via LeaveBreadcrumb()
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	bool bManual = true;

	// A navigation event, such as a window opening or closing
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	bool bNavigation = true;

	// A background process such as a database query
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	bool bProcess = true;

	// A network request
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	bool bRequest = true;

	// A change in application state, such as launch or memory warning
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	bool bState = true;

	// A user action, such as tapping a button
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	bool bUser = true;
};

USTRUCT()
struct FBugsnagErrorTypes
{
	GENERATED_BODY()

	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration", DisplayName = "Application Not Responding errors (Android only)")
	bool bANRs = true;

	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration", DisplayName = "App Hangs (iOS only)")
	bool bAppHangs = true;

	// Terminations due to excessive memory usage.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration", DisplayName = "Out Of Memory errors (iOS only)")
	bool bOOMs = true;

	// Low-level crashes such as failed assertions, invalid memory accesses, invalid instructions, uncaught C++ exceptions, etc.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	bool bNativeCrashes = true;

	// Terminations due to the device being in a critical thermal state.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration", DisplayName = "Thermal Kills (iOS only)")
	bool bThermalKills = true;

	// Unhandled exceptions from higher-level languages such as Objective-C or Java.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	bool bUnhandledExceptions = true;
};

UENUM()
enum class EBugsnagSendThreadsPolicy : uint8
{
	All,
	UnhandledOnly,
	Never
};

/**
 * Declares settings for the Bugsnag plugin in a way that can be understood by and rendered in Unreal Editor.
 * 
 * https://docs.unrealengine.com/en-US/ProgrammingAndScripting/GameplayArchitecture/Properties/
 */
UCLASS(Config = Engine, DefaultConfig)
class BUGSNAG_API UBugsnagSettings : public UObject
{
	GENERATED_UCLASS_BODY()

	///////////////////////////////////////////////////////////////////////////
	//
	//	Basic Configuration
	//
	///////////////////////////////////////////////////////////////////////////

	// Provide your project's API key here to have Bugsnag start automatically.
	// If more advanced configuration is required, start Bugsnag programatically instead.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Basic Configuration", DisplayName = "API Key")
	FString ApiKey;

	///////////////////////////////////////////////////////////////////////////
	//
	//	Advanced Configuration
	//
	///////////////////////////////////////////////////////////////////////////

	// Whether uncaught exceptions and other crashes should be reported automatically.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	bool bAutoDetectErrors = true;

	// Whether app sessions should be tracked automatically.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	bool bAutoTrackSessions = true;

	// A general summary of what was occuring in the application.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	FString Context;

	// An array of strings that determine which errors should be discarded based on their `errorClass`.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	TArray<FString> DiscardClasses = {};

	// The types of breadcrumb that should be reported.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	FBugsnagEnabledBreadcrumbTypes EnabledBreadcrumbTypes;

	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	FBugsnagErrorTypes EnabledErrorTypes;

	// Specifies which release stages errors should be sent from. Leave empty to send from all.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	TArray<FString> EnabledReleaseStages;

	// The keys whose values should be redacted from metadata before being sent to Bugsnag.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	TArray<FString> RedactedKeys = {TEXT("password")};

	// Determines whether Bugsnag should include the state of all threads at the time of an error.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	EBugsnagSendThreadsPolicy SendThreads = EBugsnagSendThreadsPolicy::All;

	// The amount of time after starting Bugsnag that should be considered part of the app's launch.
	// Events that occur during app launch will have the `app.isLaunching` property set to true.
	// Setting this to `0` will cause Bugsnag to consider the app to be launching until `MarkLaunchCompleted()` has been called.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration", DisplayName = "Launch Duration (Milliseconds)")
	uint64 LaunchDurationMillis = 5000;

	// Determines whether launch crashes should be sent synchronously when Bugsnag is started.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	bool bSendLaunchCrashesSynchronously = true;

	// The maximum number of breadcrumbs to store before deleting the oldest.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration", Meta = (ClampMax = "100"))
	uint64 MaxBreadcrumbs = 50;

	// The maximum number of events to store before deleting the oldest.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration", Meta = (ClampMin = "1"))
	uint64 MaxPersistedEvents = 32;

	// The maximum number of sessions to store before deleting the oldest.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration", Meta = (ClampMin = "1"))
	uint64 MaxPersistedSessions = 128;

	// Whether User information should be persisted to disk between application runs.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Advanced Configuration")
	bool bPersistUser = true;

	///////////////////////////////////////////////////////////////////////////
	//
	//	App Information
	//
	///////////////////////////////////////////////////////////////////////////

	// The release stage of the application, such as production, development, beta et cetera.
	// Defaults to "development" for debug builds and "production" otherwise.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "App Information")
	FString ReleaseStage;

	// Overrides `app.appType` if not empty (by default it corresponds to the underlying platform.)
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "App Information")
	FString AppType;

	// Overrides `app.appVersion` if not empty.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "App Information")
	FString AppVersion;

	// Overrides `app.bundleVersion` if not empty (iOS only.)
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "App Information", DisplayName = "Bundle Version (iOS only)")
	FString BundleVersion;

	// Overrides `app.versionCode` if not empty (Android only.)
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "App Information", DisplayName = "Version Code (Android only)")
	int VersionCode;

	///////////////////////////////////////////////////////////////////////////
	//
	//	Endpoints
	//
	///////////////////////////////////////////////////////////////////////////

	// Where error events should be sent.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Endpoints", DisplayName = "Notify")
	FString NotifyEndpoint = TEXT("https://notify.bugsnag.com");

	// Where sessions should be sent.
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Endpoints", DisplayName = "Sessions")
	FString SessionsEndpoint = TEXT("https://sessions.bugsnag.com");
};
