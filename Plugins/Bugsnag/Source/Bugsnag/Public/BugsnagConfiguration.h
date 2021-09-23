#pragma once

#include "BugsnagBreadcrumb.h"
#include "BugsnagEndpointConfiguration.h"
#include "BugsnagEvent.h"
#include "BugsnagSession.h"
#include "BugsnagSettings.h"
#include "BugsnagUser.h"
#include "LogBugsnag.h"

#include "Dom/JsonObject.h"

typedef TFunction<bool(TSharedRef<IBugsnagBreadcrumb>)> FBugsnagOnBreadcrumbCallback;
typedef TFunction<bool(TSharedRef<IBugsnagEvent>)> FBugsnagOnErrorCallback;
typedef TFunction<bool(TSharedRef<IBugsnagSession>)> FBugsnagOnSessionCallback;

class BUGSNAG_API FBugsnagConfiguration
{
public:
	FBugsnagConfiguration(const FString& ApiKey);

	static TSharedPtr<FBugsnagConfiguration> Load();

	// API Key

	const FString& GetApiKey() const { return ApiKey; }

	void SetApiKey(const FString& ApiKey);

	// Auto Detect Errors

	const bool GetAutoDetectErrors() const { return bAutoDetectErrors; }

	void SetAutoDetectErrors(bool Value) { bAutoDetectErrors = Value; }

	// Auto Track Sessions

	const bool GetAutoTrackSessions() const { return bAutoTrackSessions; }

	void SetAutoTrackSessions(bool Value) { bAutoTrackSessions = Value; }

	// Context

	const FString& GetContext() const { return Context; }

	void SetContext(const FString& Value) { Context = Value; }

	// DiscardClasses

	const TArray<FString>& GetDiscardClasses() const { return DiscardClasses; }

	void SetDiscardClasses(const TArray<FString>& Value) { DiscardClasses = Value; }

	// Enabled Breadcrumb Types

	const FBugsnagEnabledBreadcrumbTypes GetEnabledBreadcrumbTypes() const { return EnabledBreadcrumbTypes; }

	void SetEnabledBreadcrumbTypes(FBugsnagEnabledBreadcrumbTypes Value) { EnabledBreadcrumbTypes = Value; }

	// Enabled Error Types

	const FBugsnagErrorTypes GetEnabledErrorTypes() const { return EnabledErrorTypes; }

	void SetEnabledErrorTypes(FBugsnagErrorTypes Value) { EnabledErrorTypes = Value; }

	// Enabled Release Stages

	const TArray<FString>& GetEnabledReleaseStages() const { return EnabledReleaseStages; }

	void SetEnabledReleaseStages(const TArray<FString>& Value) { EnabledReleaseStages = Value; }

	// Redacted Keys

	const TArray<FString>& GetRedactedKeys() const { return RedactedKeys; }

	void SetRedactedKeys(const TArray<FString>& Value) { RedactedKeys = Value; }

	// App Hang Threshold

	uint64 GetAppHangThresholdMillis() const { return AppHangThresholdMillis; }

	void SetAppHangThresholdMillis(uint64 Value)
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

	static uint64 const AppHangThresholdFatalOnly;

	// Send Threads

	EBugsnagSendThreadsPolicy GetSendThreads() const { return SendThreads; }

	void SetSendThreads(EBugsnagSendThreadsPolicy Value) { SendThreads = Value; }

	// Launch Duration

	uint64 GetLaunchDurationMillis() const { return LaunchDurationMillis; }

	void SetLaunchDurationMillis(uint64 Value) { LaunchDurationMillis = Value; }

	// Send Launch Crashes Synchronously

	bool GetSendLaunchCrashesSynchronously() const { return bSendLaunchCrashesSynchronously; }

	void SetSendLaunchCrashesSynchronously(bool Value) { bSendLaunchCrashesSynchronously = Value; }

	// Max Breadcrumbs

	uint64 GetMaxBreadcrumbs() const { return MaxBreadcrumbs; }

	void SetMaxBreadcrumbs(uint64 Value)
	{
		if (Value > 100)
		{
			UE_LOG(LogBugsnag, Warning, TEXT("Invalid configuration. MaxBreadcrumbs should be a number between 0-100, got %lu"), Value);
		}
		else
		{
			MaxBreadcrumbs = Value;
		}
	}

	// Max Persisted Events

	uint64 GetMaxPersistedEvents() const { return MaxPersistedEvents; }

	void SetMaxPersistedEvents(uint64 Value) { MaxPersistedEvents = Value; };

	// Max Persisted Sessions

	uint64 GetMaxPersistedSessions() const { return MaxPersistedSessions; }

	void SetMaxPersistedSessions(uint64 Value) { MaxPersistedSessions = Value; };

	// Persist User

	bool GetPersistUser() const { return bPersistUser; }

	void SetPersistUser(bool Value) { bPersistUser = Value; }

	// Release Stage

	const FString& GetReleaseStage() const { return ReleaseStage; }

	void SetReleaseStage(const FString& Value) { ReleaseStage = Value; }

	// App Type

	const FString& GetAppType() const { return AppType; }

	void SetAppType(const FString& Value) { AppType = Value; }

	// App Version

	const FString& GetAppVersion() const { return AppVersion; }

	void SetAppVersion(const FString& Value) { AppVersion = Value; }

	// Bundle Version

	const FString& GetBundleVersion() const { return BundleVersion; }

	void SetBundleVersion(const FString& Value) { BundleVersion = Value; }

	// Version Code

	const FString& GetVersionCode() const { return VersionCode; }

	void SetVersionCode(const FString& Value) { VersionCode = Value; }

	// Endpoints

	const FBugsnagEndpointConfiguration& GetEndpoints() const { return Endpoints; }

	void SetEndpoints(const FString& Notify, const FString& Sessions) { Endpoints = FBugsnagEndpointConfiguration(Notify, Sessions); }

	// User

	const FBugsnagUser GetUser() const { return User; }

	void SetUser(const FString& Id = TEXT(""), const FString& Email = TEXT(""), const FString& Name = TEXT(""));

	// Metadata

	void AddMetadata(const FString& Section, const TSharedPtr<FJsonObject>& Metadata) { MetadataValues.Add(Section, Metadata); }

	void AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value);

	TSharedPtr<FJsonObject> GetMetadata(const FString& Section);

	TSharedPtr<FJsonValue> GetMetadata(const FString& Section, const FString& Key);

	void ClearMetadata(const FString& Section) { MetadataValues.Remove(Section); }

	void ClearMetadata(const FString& Section, const FString& Key);

	// Callbacks

	void AddOnBreadcrumb(const FBugsnagOnBreadcrumbCallback& Callback) { OnBreadcrumbCallbacks.Add(Callback); }

	// Android only, and only for handled errors.
	void AddOnError(const FBugsnagOnErrorCallback& Callback) { OnErrorCallbacks.Add(Callback); }

	// iOS only, may be called long after the crash occurred.
	void AddOnSendError(const FBugsnagOnErrorCallback& Callback) { OnSendErrorCallbacks.Add(Callback); }

	void AddOnSession(const FBugsnagOnSessionCallback& Callback) { OnSessionCallbacks.Add(Callback); }

private:
	FBugsnagConfiguration(const UBugsnagSettings& Settings);

	const TArray<FBugsnagOnBreadcrumbCallback>& GetOnBreadcrumbCallbacks() const { return OnBreadcrumbCallbacks; }

	const TArray<FBugsnagOnErrorCallback>& GetOnErrorCallbacks() const { return OnErrorCallbacks; }

	const TArray<FBugsnagOnErrorCallback>& GetOnSendErrorCallbacks() const { return OnSendErrorCallbacks; }

	const TArray<FBugsnagOnSessionCallback>& GetOnSessionCallbacks() const { return OnSessionCallbacks; }

	const TMap<FString, TSharedPtr<FJsonObject>>& GetMetadataValues() const { return MetadataValues; }

	friend class FApplePlatformConfiguration;

	FString ApiKey;
	bool bAutoDetectErrors = true;
	bool bAutoTrackSessions = true;
	FString Context;
	TArray<FString> DiscardClasses;
	FBugsnagEnabledBreadcrumbTypes EnabledBreadcrumbTypes;
	FBugsnagErrorTypes EnabledErrorTypes;
	TArray<FString> EnabledReleaseStages;
	TArray<FString> RedactedKeys = {TEXT("password")};
	uint64 AppHangThresholdMillis = AppHangThresholdFatalOnly;
	EBugsnagSendThreadsPolicy SendThreads = EBugsnagSendThreadsPolicy::All;
	uint64 LaunchDurationMillis = 5000;
	bool bSendLaunchCrashesSynchronously = true;
	uint64 MaxBreadcrumbs = 50;
	uint64 MaxPersistedEvents = 32;
	uint64 MaxPersistedSessions = 128;
	bool bPersistUser = true;
	FBugsnagUser User;
	FString ReleaseStage;
	FString AppType;
	FString AppVersion;
	FString BundleVersion;
	FString VersionCode;
	FBugsnagEndpointConfiguration Endpoints;
	TMap<FString, TSharedPtr<FJsonObject>> MetadataValues;
	TArray<FBugsnagOnBreadcrumbCallback> OnBreadcrumbCallbacks;
	TArray<FBugsnagOnErrorCallback> OnErrorCallbacks;
	TArray<FBugsnagOnErrorCallback> OnSendErrorCallbacks;
	TArray<FBugsnagOnSessionCallback> OnSessionCallbacks;
};
