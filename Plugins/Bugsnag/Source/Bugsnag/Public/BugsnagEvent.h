// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "BugsnagAppWithState.h"
#include "BugsnagBreadcrumb.h"
#include "BugsnagDeviceWithState.h"
#include "BugsnagError.h"
#include "BugsnagMetadataStore.h"
#include "BugsnagThread.h"
#include "BugsnagUser.h"

UENUM()
enum class EBugsnagSeverity : uint8
{
	Info,
	Warning,
	Error,
};

/**
 * Represents an occurrence of an error, along with information about the state of the app and device.
 *
 * Can be amended from an OnError callback if necessary.
 */
class BUGSNAG_API IBugsnagEvent : public virtual IBugsnagMetadataStore
{
public:
	/**
	 * The Bugsnag API key to use for this event.
	 */
	virtual const TOptional<FString> GetApiKey() const = 0;

	/**
	 * Specify a different API key if you want to send certain events to a different Bugsnag project.
	 */
	virtual void SetApiKey(const TOptional<FString>&) = 0;

	/**
	 * A string describing what was happening in your application at the time an error occurs,
	 * that given high visual prominence in the dashboard and is used to group similar errors.
	 *
	 * By default this value is null.
	 */
	virtual const TOptional<FString> GetContext() const = 0;

	virtual void SetContext(const TOptional<FString>&) = 0;

	/**
	 * The grouping hash determines which events will be grouped together. All events with the same
	 * grouping hash will be grouped together into one error.
	 *
	 * By default this value is null, which causes the Bugsnag dashboard to use its default grouping.
	 */
	virtual const TOptional<FString> GetGroupingHash() const = 0;

	/**
	 * Setting the grouping hash is an advanced usage of the library and mis-using it will cause
	 * your events not to group properly in your dashboard.
	 */
	virtual void SetGroupingHash(const TOptional<FString>&) = 0;

	/**
	 * Information about the app at the time the error ocurred.
	 */
	virtual const TSharedRef<IBugsnagAppWithState> GetApp() const = 0;

	/**
	 * Information about the computer or device running the app at the time the error ocurred.
	 */
	virtual const TSharedRef<IBugsnagDeviceWithState> GetDevice() const = 0;

	/**
	 * The breadcrumbs that lead up to this event.
	 */
	virtual const TArray<TSharedRef<IBugsnagBreadcrumb>> GetBreadcrumbs() const = 0;

	/**
	 * Information extracted from the error that caused the event.
	 *
	 * The array contains at least one error that represents the root cause, with subsequent
	 * elements representing errors that caused the preceding error.
	 */
	virtual const TArray<TSharedRef<IBugsnagError>> GetErrors() const = 0;

	/**
	 * Information about the threads at the time the error was detected, if SendThreads was enabled.
	 */
	virtual const TArray<TSharedRef<IBugsnagThread>> GetThreads() const = 0;

	/**
	 * The severity of the event.
	 *
	 * By default, this is 'Error' for unhandled errors and 'Warning' for calls to Notify().
	 */
	virtual const EBugsnagSeverity GetSeverity() const = 0;

	virtual void SetSeverity(EBugsnagSeverity) = 0;

	/**
	 * True if the error was detected automatically by Bugsnag or false if reported manually via Notify().
	 */
	virtual const bool GetUnhandled() const = 0;

	virtual void SetUnhandled(bool) = 0;

	/**
	 * Information about the user of the app at the time the error was detected.
	 */
	virtual const FBugsnagUser GetUser() const = 0;

	virtual void SetUser(
		const TOptional<FString>& Id = TOptional<FString>(),
		const TOptional<FString>& Email = TOptional<FString>(),
		const TOptional<FString>& Name = TOptional<FString>()) = 0;
};
