// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"

#include "BugsnagApp.h"
#include "BugsnagDevice.h"
#include "BugsnagUser.h"

/**
 * Represents a session of user interaction with your app.
 *
 * Can be amended from an OnError or OnSession callback if necessary.
 */
class BUGSNAG_API IBugsnagSession
{
public:
	/**
	 * A unique ID for the session.
	 */
	virtual const FString GetId() const = 0;

	virtual void SetId(const FString&) = 0;

	/**
	 * The timestamp of when the session was started.
	 */
	virtual const FDateTime GetStartedAt() const = 0;

	virtual void SetStartedAt(const FDateTime&) = 0;

	/**
	 * Information about the app.
	 */
	virtual TSharedRef<IBugsnagApp> GetApp() = 0;

	/**
	 * Information about the computer or device running the app.
	 */
	virtual TSharedRef<IBugsnagDevice> GetDevice() = 0;

	/**
	 * Information about the user of the app.
	 */
	virtual const FBugsnagUser GetUser() const = 0;

	virtual void SetUser(
		const TOptional<FString>& Id = TOptional<FString>(),
		const TOptional<FString>& Email = TOptional<FString>(),
		const TOptional<FString>& Name = TOptional<FString>()) = 0;
};
