// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "BugsnagConfiguration.h"
#include "BugsnagLastRunInfo.h"
#include "BugsnagUser.h"

#include "BugsnagFunctionLibrary.generated.h"

UCLASS()
class BUGSNAG_API UBugsnagFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Start

	/**
	 * Begin capturing events and sessions. Calling Start() (either manually or
	 * automatically through setting API key in config files) is required before
	 * using other Bugsnag functions.
	 *
	 * @param ApiKey The project API key, available on the Bugsnag dashboard
	 */
	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static void Start(const FString& ApiKey);

	/**
	 * Begin capturing events and sessions. Calling Start() (either manually or
	 * automatically through setting API key in config files) is required before
	 * using other Bugsnag functions.
	 *
	 * @param Configuration the settings to use during initialization
	 */
	static void Start(const TSharedRef<FBugsnagConfiguration>& Configuration);

	// Notify

	/**
	 * Create and send an event to Bugsnag
	 *
	 * @param ErrorClass The primary identifier or name of the event. This name (in
	 *                   addition to stack trace contents) is used to group related
	 *                   events together.
	 * @param Message    Additional details about the event
	 */
	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static void Notify(const FString& ErrorClass, const FString& Message)
	{
		Notify(ErrorClass, Message, CaptureStackTrace(), nullptr);
	};

	/**
	 * Create and send an event to Bugsnag
	 *
	 * @param ErrorClass The primary identifier or name of the event. This name (in
	 *                   addition to stack trace contents) is used to group related
	 *                   events together.
	 * @param Message    Additional details about the event
	 * @param Callback   A lambda for modifying event contents before being sent,
	 *                   such as appending additional information.
	 */
	FORCEINLINE static void Notify(const FString& ErrorClass, const FString& Message, const FBugsnagOnErrorCallback& Callback)
	{
		Notify(ErrorClass, Message, CaptureStackTrace(), Callback);
	};

	/**
	 * Create and send an event to Bugsnag
	 *
	 * @param ErrorClass The primary identifier or name of the event. This name (in
	 *                   addition to stack trace contents) is used to group related
	 *                   events together.
	 * @param Message    Additional details about the event
	 * @param StackTrace An array of frame pointers representing the call stack at
	 *                   the time of the event
	 */
	static void Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace)
	{
		Notify(ErrorClass, Message, StackTrace, nullptr);
	};

	/**
	 * Create and send an event to Bugsnag
	 *
	 * @param ErrorClass The primary identifier or name of the event. This name (in
	 *                   addition to stack trace contents) is used to group related
	 *                   events together.
	 * @param Message    Additional details about the event
	 * @param StackTrace An array of frame pointers representing the call stack at
	 *                   the time of the event
	 * @param Callback   A lambda for modifying event contents before being sent,
	 *                   such as appending additional information.
	 */
	static void Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace,
		const FBugsnagOnErrorCallback& Callback);

	// Returns the stack trace of the calling thread.
	FORCENOINLINE static TArray<uint64> CaptureStackTrace();

	// Context

	/**
	 * Bugsnag uses the concept of "contexts" to help display and group your
	 * errors. Contexts represent what was happening in your application at the
	 * time an error occurs, and is displayed prominently on the Bugsnag
	 * dashboard.
	 *
	 * @return The current context if set
	 */
	static const FString GetContext();

	/**
	 * Set a "context".
	 *
	 * Bugsnag uses the concept of "contexts" to help display and group your
	 * errors. Contexts represent what was happening in your application at the
	 * time an error occurs, and is displayed prominently on the Bugsnag
	 * dashboard.
	 *
	 * @param Context the new context to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static void SetContext(const FString& Context);

	// User

	/**
	 * Get existing user information associated with events and sessions.
	 *
	 * @return The user
	 */
	static const FBugsnagUser GetUser();

	/**
	 * Sets the user associated with an event. Calling SetUser() with no arguments
	 * clears existing data.
	 *
	 * @param Id    An identifier to differentiate this user
	 * @param Email An email addresses associated with the user
	 * @param Name  A name for the user
	 */
	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static void SetUser(const FString& Id = TEXT(""), const FString& Email = TEXT(""), const FString& Name = TEXT(""));

	// Feature flags and experiments

	/**
	 * Add a single feature flag with optional experiment membership/variant.
	 * Any existing flag with the same name will be replaced with the newer value.
	 *
	 * @param Name    The name of the feature flag to add.
	 * @param Variant The variant to set the feature flag to.
	 */
	static void AddFeatureFlag(const FString& Name, const TOptional<FString>& Variant = TOptional<FString>());

	/**
	 * Add multiple feature flags and any associated experiment membership.
	 * Existing flags with matching names will be replaced with the newer values.
	 *
	 * @param FeatureFlags An array of feature flag objects to add.
	 */
	static void AddFeatureFlags(const TArray<FBugsnagFeatureFlag>& FeatureFlags);

	/**
	 * Remove a single feature flag and any associated experiment membership.
	 *
	 * @param Name The name of the feature flag to remove.
	 */
	static void ClearFeatureFlag(const FString& Name);

	/**
	 * Remove all feature flag and experiment membership data.
	 */
	static void ClearFeatureFlags();

	// Metadata

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section  The category of information
	 * @param Metadata New or updated information to append to section
	 */
	static void AddMetadata(const FString& Section, const TSharedRef<FJsonObject>& Metadata);

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	static void AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value);

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	static void AddMetadata(const FString& Section, const FString& Key, const FString& Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueString>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	static void AddMetadata(const FString& Section, const FString& Key, const TCHAR* Value)
	{
		AddMetadata(Section, Key, MakeShareable(Value ? new FJsonValueString(Value) : nullptr));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	static void AddMetadata(const FString& Section, const FString& Key, const char* Value)
	{
		AddMetadata(Section, Key, MakeShareable(Value ? new FJsonValueString(Value) : nullptr));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	static void AddMetadata(const FString& Section, const FString& Key, double Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	static void AddMetadata(const FString& Section, const FString& Key, float Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	static void AddMetadata(const FString& Section, const FString& Key, int8 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	static void AddMetadata(const FString& Section, const FString& Key, int16 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	static void AddMetadata(const FString& Section, const FString& Key, int32 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	static void AddMetadata(const FString& Section, const FString& Key, int64 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	static void AddMetadata(const FString& Section, const FString& Key, uint8 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	static void AddMetadata(const FString& Section, const FString& Key, uint16 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	static void AddMetadata(const FString& Section, const FString& Key, uint32 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	static void AddMetadata(const FString& Section, const FString& Key, uint64 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	static void AddMetadata(const FString& Section, const FString& Key, bool Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueBoolean>(Value));
	}

	/**
	 * Retrieve cached diagnostics associated with a specific section
	 *
	 * @param Section The section to retrieve
	 *
	 * @return The associated data, if any
	 */
	static TSharedPtr<FJsonObject> GetMetadata(const FString& Section);

	/**
	 * Retrieve cached diagnostics associated with a specific section and key
	 *
	 * @param Section The section containing key
	 * @param Key     The value's identifier
	 *
	 * @return The associated data, if any
	 */
	static TSharedPtr<FJsonValue> GetMetadata(const FString& Section, const FString& Key);

	/**
	 * Remove cached diagnostics from the specified section
	 *
	 * @param Section The section to remove
	 */
	static void ClearMetadata(const FString& Section);

	/**
	 * Removed the cached value from a specific section and key
	 *
	 * @param Section The section containing key
	 * @param Key     The key to remove
	 */
	static void ClearMetadata(const FString& Section, const FString& Key);

	// Breadcrumbs

	/**
	 * Leave a "waypoint" or log message about a notable event, the most recent of
	 * which will be attached to subsequent events to aid in debugging
	 *
	 * @param Message The message contents
	 * @param Type    A general message category, which can aid in filtering
	 */
	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static void LeaveBreadcrumb(const FString& Message, EBugsnagBreadcrumbType Type = EBugsnagBreadcrumbType::Manual);

	/**
	 * Leave a "waypoint" or log message about a notable event, the most recent of
	 * which will be attached to subsequent events to aid in debugging
	 *
	 * @param Message  The message contents
	 * @param Metadata Additional diagnostics about the log
	 * @param Type     A general message category, which can aid in filtering
	 */
	static void LeaveBreadcrumb(const FString& Message, const TSharedPtr<FJsonObject>& Metadata,
		EBugsnagBreadcrumbType Type = EBugsnagBreadcrumbType::Manual);

	/**
	 * Retrieve the breadcrumbs currently stored
	 */
	static TArray<TSharedRef<const IBugsnagBreadcrumb>> GetBreadcrumbs();

	// Crashes On Launch

	/**
	 * Marks the application as having completed launching, setting
	 * app.isLaunching to false in the metadata of subsequent events. Synchronous
	 * delivery will not be attempted upon next launch for any fatal crashes.
	 *
	 * By default, this function is invoked automatically once the configured
	 * launch duration has elapsed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static void MarkLaunchCompleted();

	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static bool IsStarted();

	/**
	 * Information about the last launch of the application, if it has been run
	 * before.
	 *
	 * This allows checking whether the app crashed on its last launch, which
	 * could be used to perform conditional behaviour to recover from crashes,
	 * such as clearing the app data cache.
	 */
	static TSharedPtr<FBugsnagLastRunInfo> GetLastRunInfo();

	// Sessions

	/**
	 * Starts tracking a new session. Automatic session tracking should be
	 * disabled in configuration if using this option.
	 *
	 * Call StartSession() at a time appropriate to your application when you want
	 * to start counting events towards your project's Stability Score. This will
	 * start a new session even if there is an existing session. Call
	 * ResumeSession() to start a session only when one did not previously exist.
	 */
	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static void StartSession();

	/**
	 * Pause the tracking of a session. Any events reported after pausing will not
	 * count towards your application's Stability Score.
	 */
	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static void PauseSession();

	/**
	 * Resumes a session which has previously been paused, or starts a new session
	 * if none exists. This function has no effect if a session is already active.
	 *
	 * @return true if a previous session was resumed, false if a new session was
	 * started
	 */
	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static bool ResumeSession();

	// Filtering

	/**
	 * Add a callback which will be executed before a breadcrumb is cached. Within
	 * the callback, the breadcrumb can be modified or discarded altogether by
	 * returning false.
	 *
	 * @param Callback The callback to add
	 */
	static void AddOnBreadcrumb(FBugsnagOnBreadcrumbCallback Callback);

	/**
	 * Add a callback which will be executed before a session is sent or
	 * persisted. Within the callback, the session can be modified or discarded
	 * altogether by returning false.
	 *
	 * @param Callback The callback to add
	 */
	static void AddOnSession(FBugsnagOnSessionCallback Callback);
};
