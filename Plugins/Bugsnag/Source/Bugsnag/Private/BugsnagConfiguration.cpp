#include "BugsnagConfiguration.h"

#include "BugsnagConstants.h"

#include "Engine/Engine.h"
#include "GameFramework/GameState.h"
#include "Misc/EngineVersion.h"
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

uint64 const FBugsnagConfiguration::AppHangThresholdFatalOnly = INT_MAX;

FBugsnagConfiguration::FBugsnagConfiguration(const FString& ApiKey)
{
	SetApiKey(ApiKey);
	AddDefaultMetadata();
}

FBugsnagConfiguration::FBugsnagConfiguration(const UBugsnagSettings& Settings)
	: bAutoDetectErrors(Settings.bAutoDetectErrors)
	, bAutoTrackSessions(Settings.bAutoTrackSessions)
	, Context(UnsetIfEmpty(Settings.Context))
	, DiscardClasses(Settings.DiscardClasses)
	, EnabledBreadcrumbTypes(Settings.EnabledBreadcrumbTypes)
	, EnabledErrorTypes(Settings.EnabledErrorTypes)
	, EnabledReleaseStages(Settings.EnabledReleaseStages)
	, RedactedKeys(Settings.RedactedKeys)
	, SendThreads(Settings.SendThreads)
	, LaunchDurationMillis(Settings.LaunchDurationMillis)
	, bSendLaunchCrashesSynchronously(Settings.bSendLaunchCrashesSynchronously)
	, MaxPersistedEvents(Settings.MaxPersistedEvents)
	, MaxPersistedSessions(Settings.MaxPersistedSessions)
	, bPersistUser(Settings.bPersistUser)
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
	AddDefaultMetadata();
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

void FBugsnagConfiguration::SetUser(const TOptional<FString>& Id, const TOptional<FString>& Email, const TOptional<FString>& Name)
{
	User = FBugsnagUser(Id, Email, Name);
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
	// This logic copied from FEngineService::SendPong() in Runtime/Engine/Private/EngineService.cpp
	FWorldContext const* ContextToUse = nullptr;
	if (GEngine)
	{
		// We're going to look through the WorldContexts and pull any Game context we find
		// If there isn't a Game context, we'll take the first PIE we find
		// and if none of those we'll use an Editor
		for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
		{
			if (WorldContext.WorldType == EWorldType::Game)
			{
				ContextToUse = &WorldContext;
				break;
			}
			else if (WorldContext.WorldType == EWorldType::PIE && (ContextToUse == nullptr || ContextToUse->WorldType != EWorldType::PIE))
			{
				ContextToUse = &WorldContext;
			}
			else if (WorldContext.WorldType == EWorldType::Editor && ContextToUse == nullptr)
			{
				ContextToUse = &WorldContext;
			}
		}
	}
	return ContextToUse ? ContextToUse->World() : nullptr;
}

void FBugsnagConfiguration::AddDefaultMetadata()
{
	TSharedRef<FJsonObject> DeviceMetadata = MakeShared<FJsonObject>();

	if (!GRHIAdapterName.IsEmpty())
	{
		DeviceMetadata->SetStringField(BugsnagConstants::AdapterName, *GRHIAdapterName);
	}

	if (!GRHIAdapterInternalDriverVersion.IsEmpty())
	{
		DeviceMetadata->SetStringField(BugsnagConstants::DriverVersion, *GRHIAdapterInternalDriverVersion);
	}

	AddMetadata(BugsnagConstants::Device, DeviceMetadata);

	//

	TSharedRef<FJsonObject> EngineMetadata = MakeShared<FJsonObject>();

	EngineMetadata->SetStringField(BugsnagConstants::Version, FEngineVersion::Current().ToString(EVersionComponent::Changelist));

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
