// Copyright 2021 Bugsnag. All Rights Reserved.

#include "BugsnagConfiguration.h"

#include "BugsnagConstants.h"
#include "BugsnagUtils.h"
#include "LogBugsnag.h"

#include "Engine/Engine.h"
#include "GameFramework/GameState.h"
#include "RHI.h"
#include "UserActivityTracking.h"

static TOptional<FString> UnsetIfEmpty(const FString& Value)
{
	return Value.IsEmpty() ? TOptional<FString>() : TOptional<FString>(Value);
}

template <typename T>
static TOptional<T> UnsetIfZero(const T Value)
{
	return Value == 0 ? TOptional<T>() : Value;
}

static EBugsnagEnabledBreadcrumbTypes Convert(FBugsnagEnabledBreadcrumbTypes Value)
{
	return (Value.bError ? EBugsnagEnabledBreadcrumbTypes::Error : EBugsnagEnabledBreadcrumbTypes::None) |
		   (Value.bLog ? EBugsnagEnabledBreadcrumbTypes::Log : EBugsnagEnabledBreadcrumbTypes::None) |
		   (Value.bNavigation ? EBugsnagEnabledBreadcrumbTypes::Navigation : EBugsnagEnabledBreadcrumbTypes::None) |
		   (Value.bProcess ? EBugsnagEnabledBreadcrumbTypes::Process : EBugsnagEnabledBreadcrumbTypes::None) |
		   (Value.bRequest ? EBugsnagEnabledBreadcrumbTypes::Request : EBugsnagEnabledBreadcrumbTypes::None) |
		   (Value.bState ? EBugsnagEnabledBreadcrumbTypes::State : EBugsnagEnabledBreadcrumbTypes::None) |
		   (Value.bUser ? EBugsnagEnabledBreadcrumbTypes::User : EBugsnagEnabledBreadcrumbTypes::None);
}

static EBugsnagTelemetryTypes Convert(const FBugsnagTelemetryTypes& Value)
{
	return (Value.bInternalErrors ? EBugsnagTelemetryTypes::InternalErrors : EBugsnagTelemetryTypes::None) |
		   (Value.bUsage ? EBugsnagTelemetryTypes::Usage : EBugsnagTelemetryTypes::None);
}

uint64 const FBugsnagConfiguration::AppHangThresholdFatalOnly = INT_MAX;

FBugsnagConfiguration::FBugsnagConfiguration(const FString& ApiKey)
{
	SetApiKey(ApiKey);
	AddDefaults();
}

FBugsnagConfiguration::FBugsnagConfiguration(const UBugsnagSettings& Settings)
	: bAutoDetectErrors(Settings.bAutoDetectErrors)
	, bAutoTrackSessions(Settings.bAutoTrackSessions)
	, Context(UnsetIfEmpty(Settings.Context))
	, DiscardClasses(Settings.DiscardClasses)
	, EnabledBreadcrumbTypes(Convert(Settings.EnabledBreadcrumbTypes))
	, EnabledErrorTypes(Settings.EnabledErrorTypes)
	, EnabledReleaseStages(Settings.EnabledReleaseStages)
	, RedactedKeys(Settings.RedactedKeys)
	, SendThreads(Settings.SendThreads)
	, LaunchDurationMillis(Settings.LaunchDurationMillis)
	, bSendLaunchCrashesSynchronously(Settings.bSendLaunchCrashesSynchronously)
	, MaxPersistedEvents(Settings.MaxPersistedEvents)
	, MaxPersistedSessions(Settings.MaxPersistedSessions)
	, MaxStringValueLength(Settings.MaxStringValueLength)
	, bPersistUser(Settings.bPersistUser)
	, Telemetry(Convert(Settings.Telemetry))
	, ReleaseStage(UnsetIfEmpty(Settings.ReleaseStage))
	, AppType(UnsetIfEmpty(Settings.AppType))
	, AppVersion(UnsetIfEmpty(Settings.AppVersion))
	, BundleVersion(UnsetIfEmpty(Settings.BundleVersion))
	, VersionCode(UnsetIfZero(Settings.VersionCode))
	, Endpoints(FBugsnagEndpointConfiguration(Settings.NotifyEndpoint, Settings.SessionsEndpoint))
{
	if (!Settings.ApiKey.IsEmpty())
	{
		SetApiKey(Settings.ApiKey);
	}
	SetMaxBreadcrumbs(Settings.MaxBreadcrumbs);
	AddDefaults();
}

TSharedPtr<FBugsnagConfiguration> FBugsnagConfiguration::Load()
{
	const UBugsnagSettings* Settings = GetDefault<UBugsnagSettings>();
	check(Settings != nullptr);
	if (!Settings)
	{
		return nullptr;
	}
	return TSharedPtr<FBugsnagConfiguration>(new FBugsnagConfiguration(*Settings));
}

static bool IsValidApiKey(const FString& ApiKey)
{
	if (ApiKey.Len() != 32)
	{
		return false;
	}
	for (int32 i = 0; i < ApiKey.Len(); i++)
	{
		TCHAR c = ApiKey[i];
		if (!('0' <= c && c <= '9') &&
			!('A' <= c && c <= 'F') &&
			!('a' <= c && c <= 'f'))
		{
			return false;
		}
	}
	return true;
}

void FBugsnagConfiguration::SetApiKey(const FString& Value)
{
	// "Attempting to initialize Bugsnag with a null or empty apiKey shall be impossible, or loudly fail with an exception depending on the platform."
	// Check assertions halt execution in Debug, Development, Test, and Shipping Editor builds.
	// https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/Assertions/
	checkf(!Value.IsEmpty(), TEXT("No Bugsnag API Key set"));

	if (!IsValidApiKey(Value))
	{
		UE_LOG(LogBugsnag, Warning, TEXT("Invalid configuration. ApiKey should be a string with 32 hexadecimal characters, got %s"), *Value);
	}

	ApiKey = Value;
}

void FBugsnagConfiguration::SetAppHangThresholdMillis(uint64 Value)
{
	if (Value < 250)
	{
		UE_LOG(LogBugsnag, Warning,
			TEXT("Invalid configuration. AppHangThresholdMillis should be a number greater than or equal to 250, got %lu"), Value);
	}
	else
	{
		AppHangThresholdMillis = Value;
	}
}

void FBugsnagConfiguration::SetMaxBreadcrumbs(uint32 Value)
{
	if (Value > 500)
	{
		UE_LOG(LogBugsnag, Warning, TEXT("Invalid configuration. MaxBreadcrumbs should be a number between 0-500, got %lu"), Value);
	}
	else
	{
		MaxBreadcrumbs = Value;
	}
}

void FBugsnagConfiguration::SetUser(const TOptional<FString>& Id, const TOptional<FString>& Email, const TOptional<FString>& Name)
{
	User = FBugsnagUser(Id, Email, Name);
}

///////////////////////////////////////////////////////////////////////////
//
//	Feature Flags
//
///////////////////////////////////////////////////////////////////////////

void FBugsnagConfiguration::AddFeatureFlag(const FString& Name, const TOptional<FString>& Variant)
{
	FeatureFlags.Add(Name, Variant);
}

void FBugsnagConfiguration::AddFeatureFlags(const TArray<FBugsnagFeatureFlag>& FlagsToAdd)
{
	for (const FBugsnagFeatureFlag& FeatureFlag : FlagsToAdd)
	{
		FeatureFlags.Add(FeatureFlag.GetName(), FeatureFlag.GetVariant());
	}
}

void FBugsnagConfiguration::ClearFeatureFlag(const FString& Name)
{
	FeatureFlags.Remove(Name);
}

void FBugsnagConfiguration::ClearFeatureFlags()
{
	FeatureFlags.Reset();
}

///////////////////////////////////////////////////////////////////////////
//
//	Metadata
//
///////////////////////////////////////////////////////////////////////////

void FBugsnagConfiguration::AddMetadata(const FString& Section, const TSharedRef<FJsonObject>& Metadata)
{
	if (MetadataValues.Contains(Section))
	{
		MetadataValues[Section]->Values.Append(Metadata->Values);
	}
	else
	{
		TSharedRef<FJsonObject> MetadataCopy = MakeShared<FJsonObject>();
		MetadataCopy->Values = Metadata->Values;
		MetadataValues.Add(Section, MetadataCopy);
	}
}

void FBugsnagConfiguration::AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value)
{
	if (!Value.IsValid())
	{
		ClearMetadata(Section, Key);
		return;
	}
	if (MetadataValues.Contains(Section))
	{
		TSharedRef<FJsonObject> Object = MetadataValues[Section];
		Object->SetField(Key, Value);
	}
	else
	{
		TSharedRef<FJsonObject> Object = MakeShared<FJsonObject>();
		Object->SetField(Key, Value);
		MetadataValues.Add(Section, Object);
	}
}

void FBugsnagConfiguration::ClearMetadata(const FString& Section, const FString& Key)
{
	if (MetadataValues.Contains(Section))
	{
		TSharedPtr<FJsonObject> Object = MetadataValues[Section];
		Object->RemoveField(Key);
	}
}

TSharedPtr<FJsonObject> FBugsnagConfiguration::GetMetadata(const FString& Section)
{
	if (MetadataValues.Contains(Section))
	{
		return MetadataValues[Section];
	}
	else
	{
		return nullptr;
	}
}

TSharedPtr<FJsonValue> FBugsnagConfiguration::GetMetadata(const FString& Section, const FString& Key)
{
	if (MetadataValues.Contains(Section))
	{
		return MetadataValues[Section]->TryGetField(Key);
	}
	else
	{
		return nullptr;
	}
}

// Because UEngine::GetCurrentPlayWorld() does not exist in 4.23 :-/
static UWorld* GetCurrentPlayWorld()
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

void FBugsnagConfiguration::AddDefaults()
{
	if (!ReleaseStage.IsSet())
	{
#if UE_BUILD_SHIPPING
		ReleaseStage = TEXT("production");
#else
		ReleaseStage = TEXT("development");
#endif
	}

	TSharedRef<FJsonObject> DeviceMetadata = MakeShared<FJsonObject>();

	if (!GRHIAdapterName.IsEmpty())
	{
		DeviceMetadata->SetStringField(BugsnagConstants::GPUAdapterName, *GRHIAdapterName);
	}

	if (!GRHIAdapterInternalDriverVersion.IsEmpty())
	{
		DeviceMetadata->SetStringField(BugsnagConstants::GPUDriverVersion, *GRHIAdapterInternalDriverVersion);
	}

	AddMetadata(BugsnagConstants::Device, DeviceMetadata);

	//

	TSharedRef<FJsonObject> EngineMetadata = MakeShared<FJsonObject>();

	EngineMetadata->SetStringField(BugsnagConstants::Version, BugsnagUtils::GetUnrealEngineVersion());

	UWorld* CurrentPlayWorld = GetCurrentPlayWorld();
	if (CurrentPlayWorld)
	{
		if (CurrentPlayWorld->GetGameState())
		{
			EngineMetadata->SetStringField(BugsnagConstants::GameStateName, CurrentPlayWorld->GetGameState()->GetClass()->GetName());
		}
		EngineMetadata->SetStringField(BugsnagConstants::MapUrl, CurrentPlayWorld->GetLocalURL());
	}

	EngineMetadata->SetStringField(BugsnagConstants::UserActivity, FUserActivityTracking::GetUserActivity().ActionName);

	AddMetadata(BugsnagConstants::UnrealEngine, EngineMetadata);
}
