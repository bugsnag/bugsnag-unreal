// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "BugsnagBreadcrumb.h"
#include "BugsnagEndpointConfiguration.h"
#include "BugsnagEvent.h"
#include "BugsnagMetadataStore.h"
#include "BugsnagSession.h"
#include "BugsnagSettings.h"
#include "BugsnagUser.h"

#include "Dom/JsonObject.h"

/**
 * A callback for filtering or modifying breadcrumbs.
 *
 * @returns True if the breadcrumb should be recorded.
 */
typedef TFunction<bool(TSharedRef<IBugsnagBreadcrumb>)> FBugsnagOnBreadcrumbCallback;

/**
 * A callback for filtering or modifying events.
 *
 * @returns True if the event should be reported.
 */
typedef TFunction<bool(TSharedRef<IBugsnagEvent>)> FBugsnagOnErrorCallback;

/**
 * A callback for filtering or modifying sessions.
 *
 * @returns True if the session should be reported.
 */
typedef TFunction<bool(TSharedRef<IBugsnagSession>)> FBugsnagOnSessionCallback;

/**
 * Configures the behaviour of Bugsnag.
 */
class BUGSNAG_API FBugsnagConfiguration final : public IBugsnagMetadataStore
{
public:
	/**
	 * Constructs a new configuration object with default values.
	 *
	 * @param ApiKey The project API key, available on the Bugsnag dashboard.
	 */
	FBugsnagConfiguration(const FString& ApiKey);

	/**
	 * Loads a configuration object from the settings specified in `Config/DefaultEngine.ini`
	 */
	static TSharedPtr<FBugsnagConfiguration> Load();

	/**
	 * The API key to be used for events sent to Bugsnag.
	 */
	const FString& GetApiKey() const { return ApiKey; }

	/**
	 * @param ApiKey The project API key, available on the Bugsnag dashboard.
	 */
	void SetApiKey(const FString& ApiKey);

	/**
	 * By default, Bugsnag will automatically detect and report errors from your application.
	 *
	 * Use this flag to disable all automatic detection.
	 */
	const bool GetAutoDetectErrors() const { return bAutoDetectErrors; }

	/**
	 * @param Value True to enable or false to disable automatic detection of errors.
	 */
	void SetAutoDetectErrors(bool Value) { bAutoDetectErrors = Value; }

	/**
	 * By default, Bugsnag will automatically track and report session information from your application.
	 *
	 * Use this flag to disable all automatic reporting or if you want control over what is deemed a session.
	 */
	const bool GetAutoTrackSessions() const { return bAutoTrackSessions; }

	/**
	 * @param Value True to enable or false to disable automatic session tracking.
	 */
	void SetAutoTrackSessions(bool Value) { bAutoTrackSessions = Value; }

	/**
	 * Bugsnag uses the concept of "contexts" to help display and group your errors.
	 *
	 * Contexts represent what was happening in your application at the time an error occurs,
	 * and are displayed prominently on the Bugsnag dashboard.
	 */
	const TOptional<FString>& GetContext() const { return Context; }

	/**
	 * @param Value The context to include in all events (until a new context is set.)
	 */
	void SetContext(const TOptional<FString>& Value) { Context = Value; }

	/**
	 * An array of strings that determine which errors should be discarded based on their `errorClass`.
	 */
	const TArray<FString>& GetDiscardClasses() const { return DiscardClasses; }

	/**
	 * Specifies an array of strings that determine which errors should be discarded based on their `errorClass`.
	 *
	 * Values must exactly match an `errorClass` (in its entirety and case sensitive) for its event to be discarded.
	 *
	 * Events will be discarded before any `OnSendError` callbacks are invoked.
	 */
	void SetDiscardClasses(const TArray<FString>& Value) { DiscardClasses = Value; }

	/**
	 * By default Bugsnag will automatically add breadcrumbs for common application events whilst your application is running.
	 *
	 * This option determines which of these are enabled and sent to Bugsnag.
	 */
	const EBugsnagEnabledBreadcrumbTypes GetEnabledBreadcrumbTypes() const { return EnabledBreadcrumbTypes; }

	/**
	 * @param Value The breadcrumb types which should be automatically recorded.
	 */
	void SetEnabledBreadcrumbTypes(EBugsnagEnabledBreadcrumbTypes Value) { EnabledBreadcrumbTypes = Value; }

	/**
	 * Bugsnag will automatically detect different types of error in your application.
	 *
	 * This option controls exactly which types are enabled.
	 */
	const FBugsnagErrorTypes GetEnabledErrorTypes() const { return EnabledErrorTypes; }

	/**
	 * @param Value The error types which should be automatically detected.
	 */
	void SetEnabledErrorTypes(FBugsnagErrorTypes Value) { EnabledErrorTypes = Value; }

	/**
	 * By default, Bugsnag will be notified of events that happen in any releaseStage.
	 *
	 * This option controls which release stages notify Bugsnag.
	 */
	const TArray<FString>& GetEnabledReleaseStages() const { return EnabledReleaseStages; }

	/**
	 * @param Value The release stages which notify Bugsnag.
	 */
	void SetEnabledReleaseStages(const TArray<FString>& Value) { EnabledReleaseStages = Value; }

	/**
	 * Determines which values should be removed from any metadata before sending them to Bugsnag.
	 *
	 * Change this if you want to ensure you don’t transmit sensitive data such as passwords and credit card numbers.
	 *
	 * Any property whose key matches a redacted key will be filtered and replaced with `"[REDACTED]"`.
	 *
	 * By default, this array contains `"password"`.
	 *
	 * Be aware that if you supply a new value, it will replace the default, so include `"password"` in your own array
	 * if you still want to redact that.
	 */
	const TArray<FString>& GetRedactedKeys() const { return RedactedKeys; }

	/**
	 * @param Value The metadata keys whose values should be replaced with `"[REDACTED]"`
	 */
	void SetRedactedKeys(const TArray<FString>& Value) { RedactedKeys = Value; }

	/**
	 * The minimum number of milliseconds of main thread unresponsiveness that will trigger the detection and reporting
	 * of an app hang on iOS & macOS.
	 *
	 * Hangs that occur on Unreal Engine's game thread are not detected.
	 *
	 * By default this is set to `AppHangThresholdFatalOnly` which means that an error will only be sent to Bugsnag if
	 * the hang is fatal. Setting this to any other value enables reporting of non-fatal hangs from which your app
	 * recovered before being terminated by the user or the iOS watchdog.
	 */
	uint64 GetAppHangThresholdMillis() const { return AppHangThresholdMillis; }

	/**
	 * @param Value The minimum number of milliseconds of main thread unresponsiveness that should trigger the detection
	 *              and reporting of an app hang on iOS & macOS. The minimum supported value is 250.
	 */
	void SetAppHangThresholdMillis(uint64 Value);

	static uint64 const AppHangThresholdFatalOnly;

	/**
	 * Determines whether Bugsnag should send the state of all threads at the time of an error.
	 */
	EBugsnagSendThreadsPolicy GetSendThreads() const { return SendThreads; }

	/**
	 * @param Value The thread sending policy.
	 */
	void SetSendThreads(EBugsnagSendThreadsPolicy Value) { SendThreads = Value; }

	/**
	 * The amount of time (in milliseconds) after starting Bugsnag that should be considered part of the app's launch.
	 *
	 * Events that occur during app launch will have the `app.isLaunching` property set to true.
	 *
	 * By default this value is 5000 milliseconds.
	 */
	uint64 GetLaunchDurationMillis() const { return LaunchDurationMillis; }

	/**
	 * @param Value The number of milliseconds after starting Bugsnag that should be considered part of the app's launch.
	 *              Set this to `0` to cause Bugsnag to consider the app to be launching until `MarkLaunchCompleted()`
	 *              has been called.
	 */
	void SetLaunchDurationMillis(uint64 Value) { LaunchDurationMillis = Value; }

	/**
	 * Determines whether launch crashes should be sent synchronously when Bugsnag starts.
	 *
	 * If true and the previous run terminated due to a crash during app launch, Bugsnag will block the calling thread
	 * when started for up to 2 seconds while the crash report is sent.
	 *
	 * By default this value is true.
	 */
	bool GetSendLaunchCrashesSynchronously() const { return bSendLaunchCrashesSynchronously; }

	/**
	 * @param Value `true` to send launch crashes synchronously when Bugsnag starts or `false` to send them asynchronously.
	 */
	void SetSendLaunchCrashesSynchronously(bool Value) { bSendLaunchCrashesSynchronously = Value; }

	/**
	 * The maximum number of breadcrumbs which will be stored.
	 *
	 * Once the threshold is reached, the oldest breadcrumbs will be deleted.
	 *
	 * By default, 25 breadcrumbs are stored.
	 */
	uint32 GetMaxBreadcrumbs() const { return MaxBreadcrumbs; }

	/**
	 * @param Value The maximum number of breadcrumbs to store.
	 *              The maximum supported value is 100.
	 */
	void SetMaxBreadcrumbs(uint32 Value);

	/**
	 * The maximum number of events which will be stored.
	 *
	 * Once the threshold is reached, the oldest events will be deleted.
	 *
	 * By default, 32 events are stored.
	 */
	uint32 GetMaxPersistedEvents() const { return MaxPersistedEvents; }

	/**
	 * @param Value The maximum number of events to store.
	 */
	void SetMaxPersistedEvents(uint32 Value) { MaxPersistedEvents = Value; };

	/**
	 * The maximum number of sessions which will be stored.
	 *
	 * Once the threshold is reached, the oldest sessions will be deleted.
	 *
	 * By default, 128 sessions are stored.
	 */
	uint32 GetMaxPersistedSessions() const { return MaxPersistedSessions; }

	/**
	 * @param Value The maximum number of sessions to store.
	 */
	void SetMaxPersistedSessions(uint32 Value) { MaxPersistedSessions = Value; };

	/**
	 * Whether User information should be persisted to disk between application runs.
	 *
	 * Defaults to true.
	 */
	bool GetPersistUser() const { return bPersistUser; }

	/**
	 * @param Value `true` to store user information on disk between application runs, `false` otherwise.
	 */
	void SetPersistUser(bool Value) { bPersistUser = Value; }

	/**
	 * The release stage of the application, such as `"production"`, `"development"`, `"beta"` etc.
	 */
	const TOptional<FString>& GetReleaseStage() const { return ReleaseStage; }

	/**
	 * @param Value The release stage of the application, such as `"production"`, `"development"`, `"beta"` etc.
	 */
	void SetReleaseStage(const TOptional<FString>& Value) { ReleaseStage = Value; }

	/**
	 * The application type, e.g. `"android"`, `"iOS"` or `"macOS"`.
	 */
	const TOptional<FString>& GetAppType() const { return AppType; }

	/**
	 * Sets the application type.
	 *
	 * By default Bugsnag will send `"android"` on Android, `"iOS"` on iOS and `"macOS"` on macOS.
	 *
	 * If your app's codebase contains different entry-points or processes, but reports to a single Bugsnag project,
	 * you might want to add information denoting the type of process the error came from.
	 *
	 * This information can be used to filter errors in Bugsnag's dashboard.
	 */
	void SetAppType(const TOptional<FString>& Value) { AppType = Value; }

	/**
	 * The app's version string.
	 *
	 * By default Bugsnag uses the value from your app's Android manifest or Info.plist.
	 */
	const TOptional<FString>& GetAppVersion() const { return AppVersion; }

	/**
	 * @param Value the app's version string to include in events and sessions.
	 */
	void SetAppVersion(const TOptional<FString>& Value) { AppVersion = Value; }

	/**
	 * The app's bundle version (iOS & macOS only.)
	 *
	 * By default Bugsnag uses the value of `CFBundleVersion` in your app's `Info.plist`.
	 */
	const TOptional<FString>& GetBundleVersion() const { return BundleVersion; }

	/**
	 * @param Value the app's bundle version to include in events and sessions (iOS & macOS only.)
	 */
	void SetBundleVersion(const TOptional<FString>& Value) { BundleVersion = Value; }

	/**
	 * The endpoints where events and sessions will be sent.
	 *
	 * By default we'll send events to https://notify.bugsnag.com and sessions to https://sessions.bugsnag.com
	 */
	const FBugsnagEndpointConfiguration& GetEndpoints() const { return Endpoints; }

	/**
	 * Sets the endpoints where events and sessions will be sent
	 *
	 * If you are using Bugsnag Enterprise, set these to your own Bugsnag endpoints.
	 */
	void SetEndpoints(const FString& Notify, const FString& Sessions) { Endpoints = FBugsnagEndpointConfiguration(Notify, Sessions); }

	/**
	 * The directory where event and session payloads will be persisted if a network request is not successful
	 * (Android only.)
	 *
	 * The persistence directory also stores user information if `PersistUser` is set to true.
	 *
	 * By default Bugsnag uses `Context#getCacheDir()`.
	 */
	const TOptional<FString>& GetPersistenceDirectory() const { return PersistenceDirectory; }

	/**
	 * Sets the directory where event and session payloads will be persisted if a network request is not successful
	 * (Android only.)
	 *
	 * If you use Bugsnag in multiple processes, then a unique persistence directory **must** be configured for each
	 * process to prevent duplicate requests being made by each instantiation of Bugsnag.
	 */
	void SetPersistenceDirectory(const TOptional<FString>& Value) { PersistenceDirectory = Value; }

	/**
	 * The names of packages Bugsnag should consider to be part of the running application (Android only.)
	 *
	 * Stack frames are marked as in-project if they originate from any of these packages, which allows us to improve
	 * the visual display of the stacktrace on the dashboard.
	 *
	 * By default this is set to the name of the package that started Bugsnag.
	 */
	const TArray<FString>& GetProjectPackages() const { return ProjectPackages; }

	/**
	 * Sets which packages Bugsnag should consider to be part of the the running application (Android only.)
	 */
	void SetProjectPackages(const TArray<FString>& Value) { ProjectPackages = Value; }

	/**
	 * The app's `versionCode` (Android only.)
	 *
	 * We'll use the `versionCode` from your `AndroidManifest.xml` if this is not set.
	 */
	const TOptional<int32>& GetVersionCode() const { return VersionCode; }

	/**
	 * Sets the app's `versionCode` (Android only.)
	 */
	void SetVersionCode(const TOptional<int32>& Value) { VersionCode = Value; }

	/**
	 * The user associated with events and sessions.
	 */
	const FBugsnagUser GetUser() const { return User; }

	/**
	 * Sets the user associated with events and sessions.
	 *
	 * Calling `SetUser()` with no arguments clears existing data.
	 *
	 * @param Id    An identifier to differentiate this user.
	 * @param Email An email addresses associated with the user.
	 * @param Name  A name for the user.
	 */
	void SetUser(
		const TOptional<FString>& Id = TOptional<FString>(),
		const TOptional<FString>& Email = TOptional<FString>(),
		const TOptional<FString>& Name = TOptional<FString>());

	// Metadata

	using IBugsnagMetadataStore::AddMetadata;

	/**
	 * Caches additional diagnostics to be sent with any subsequent events.
	 *
	 * @param Section  The category of information
	 * @param Metadata New or updated information to append to section
	 */
	void AddMetadata(const FString& Section, const TSharedRef<FJsonObject>& Metadata) override;

	/**
	 * Caches additional diagnostics to be sent with any subsequent events.
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	void AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value) override;

	/**
	 * Retrieves cached diagnostics associated with a specific section.
	 *
	 * @param Section The section to retrieve
	 *
	 * @return The associated data, if any
	 */
	TSharedPtr<FJsonObject> GetMetadata(const FString& Section) override;

	/**
	 * Retrieves cached diagnostics associated with a specific section and key.
	 *
	 * @param Section The section containing key
	 * @param Key     The value's identifier
	 *
	 * @return The associated data, if any
	 */
	TSharedPtr<FJsonValue> GetMetadata(const FString& Section, const FString& Key) override;

	/**
	 * Removes the cached value from a specific section and key.
	 *
	 * @param Section The section containing key
	 */
	void ClearMetadata(const FString& Section) override { MetadataValues.Remove(Section); }

	/**
	 * Removes the cached value from a specific section and key.
	 *
	 * @param Section The section containing key
	 * @param Key     The key to remove
	 */
	void ClearMetadata(const FString& Section, const FString& Key) override;

	/**
	 * Adds a callback to modify or discard breadcrumbs before they are recorded.
	 *
	 * The callback should return `true` to allow or `false` to prevent the breadcrumb being recorded.
	 */
	void AddOnBreadcrumb(FBugsnagOnBreadcrumbCallback Callback) { OnBreadcrumbCallbacks.Add(Callback); }

	/**
	 * Adds a callback to modify or discard events before they are sent.
	 *
	 * The callback should return `true` to allow or `false` to prevent the event being sent.
	 *
	 * Note that the callback will not normally be invoked on the thread where the error ocurred,
	 * and may run at a much later point in time or after the application has been relaunched.
	 */
	void AddOnSendError(FBugsnagOnErrorCallback Callback) { OnSendErrorCallbacks.Add(Callback); }

	/**
	 * Adds a callback to modify or discard sessions before they are recorded.
	 *
	 * The callback should return `true` to allow or `false` to prevent the session being recorded.
	 */
	void AddOnSession(FBugsnagOnSessionCallback Callback) { OnSessionCallbacks.Add(Callback); }

	const TArray<FBugsnagOnBreadcrumbCallback>& GetOnBreadcrumbCallbacks() const { return OnBreadcrumbCallbacks; }

	const TArray<FBugsnagOnErrorCallback>& GetOnSendErrorCallbacks() const { return OnSendErrorCallbacks; }

	const TArray<FBugsnagOnSessionCallback>& GetOnSessionCallbacks() const { return OnSessionCallbacks; }

	const TMap<FString, TSharedRef<FJsonObject>>& GetMetadataValues() const { return MetadataValues; }

private:
	FBugsnagConfiguration(const UBugsnagSettings& Settings);

	void AddDefaultMetadata();

	friend class FApplePlatformConfiguration;
	friend class FAndroidPlatformConfiguration;

	FString ApiKey;
	bool bAutoDetectErrors = true;
	bool bAutoTrackSessions = true;
	TOptional<FString> Context;
	TArray<FString> DiscardClasses;
	EBugsnagEnabledBreadcrumbTypes EnabledBreadcrumbTypes = EBugsnagEnabledBreadcrumbTypes::All;
	FBugsnagErrorTypes EnabledErrorTypes;
	TArray<FString> EnabledReleaseStages;
	TArray<FString> RedactedKeys = {TEXT("password")};
	uint64 AppHangThresholdMillis = AppHangThresholdFatalOnly;
	EBugsnagSendThreadsPolicy SendThreads = EBugsnagSendThreadsPolicy::All;
	uint64 LaunchDurationMillis = 5000;
	bool bSendLaunchCrashesSynchronously = true;
	uint32 MaxBreadcrumbs = 50;
	uint32 MaxPersistedEvents = 32;
	uint32 MaxPersistedSessions = 128;
	bool bPersistUser = true;
	FBugsnagUser User;
	TOptional<FString> ReleaseStage;
	TOptional<FString> AppType;
	TOptional<FString> AppVersion;
	TOptional<FString> BundleVersion;
	TOptional<FString> PersistenceDirectory;
	TArray<FString> ProjectPackages;
	TOptional<int32> VersionCode;
	FBugsnagEndpointConfiguration Endpoints;
	TMap<FString, TSharedRef<FJsonObject>> MetadataValues;
	TArray<FBugsnagOnBreadcrumbCallback> OnBreadcrumbCallbacks;
	TArray<FBugsnagOnErrorCallback> OnSendErrorCallbacks;
	TArray<FBugsnagOnSessionCallback> OnSessionCallbacks;
};
