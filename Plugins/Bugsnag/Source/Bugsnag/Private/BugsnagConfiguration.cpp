#include "BugsnagConfiguration.h"

uint64 const FBugsnagConfiguration::AppHangThresholdFatalOnly = INT_MAX;

FBugsnagConfiguration::FBugsnagConfiguration(const FString& ApiKey)
{
	SetApiKey(ApiKey);
}

FBugsnagConfiguration::FBugsnagConfiguration(const UBugsnagSettings& Settings)
	: bAutoDetectErrors(Settings.bAutoDetectErrors)
	, bAutoTrackSessions(Settings.bAutoTrackSessions)
	, Context(Settings.Context)
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
	, ReleaseStage(Settings.ReleaseStage)
	, AppType(Settings.AppType)
	, AppVersion(Settings.AppVersion)
	, BundleVersion(Settings.BundleVersion)
	, VersionCode(Settings.VersionCode)
	, Endpoints(FBugsnagEndpointConfiguration(Settings.NotifyEndpoint, Settings.SessionsEndpoint))
{
	if (!Settings.ApiKey.IsEmpty())
	{
		SetApiKey(Settings.ApiKey);
	}
	SetMaxBreadcrumbs(Settings.MaxBreadcrumbs);
}

FBugsnagConfiguration* FBugsnagConfiguration::Load()
{
	const UBugsnagSettings* Settings;
	verify((Settings = GetDefault<UBugsnagSettings>()));
	if (!Settings)
	{
		return nullptr;
	}
	return new FBugsnagConfiguration(*Settings);
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

void FBugsnagConfiguration::SetUser(const FString& Id, const FString& Email, const FString& Name)
{
	User = FBugsnagUser(Id, Email, Name);
}

///////////////////////////////////////////////////////////////////////////
//
//	Metadata
//
///////////////////////////////////////////////////////////////////////////

void FBugsnagConfiguration::AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value)
{
	if (!Value.IsValid())
	{
		ClearMetadata(Section, Key);
		return;
	}
	if (MetadataValues.Contains(Section))
	{
		TSharedPtr<FJsonObject> Object = MetadataValues[Section];
		Object->SetField(Key, Value);
	}
	else
	{
		TSharedPtr<FJsonObject> Object = MakeShareable(new FJsonObject);
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
