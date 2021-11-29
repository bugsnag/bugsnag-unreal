// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Misc/DateTime.h"
#include "UObject/ObjectMacros.h"

UENUM()
enum class EBugsnagBreadcrumbType : uint8
{
	Manual,
	Error,
	Log,
	Navigation,
	Process,
	Request,
	State,
	User
};

enum class EBugsnagEnabledBreadcrumbTypes : uint8
{
	None = 0,
	Error = 1 << 0,
	Log = 1 << 1,
	Navigation = 1 << 2,
	Process = 1 << 3,
	Request = 1 << 4,
	State = 1 << 5,
	User = 1 << 6,
	All = Error | Log | Navigation | Process | Request | State | User
};
ENUM_CLASS_FLAGS(EBugsnagEnabledBreadcrumbTypes)

/**
 * A short log message, representing an action that occurred in your app, to aid with debugging.
 */
class BUGSNAG_API IBugsnagBreadcrumb
{
public:
	/**
	 * The timestamp that the breadcrumb was left.
	 */
	virtual const FDateTime GetTimestamp() const = 0;

	/**
	 * The description of the breadcrumb.
	 */
	virtual const FString GetMessage() const = 0;

	virtual void SetMessage(const FString&) = 0;

	/**
	 * The type of breadcrumb left.
	 */
	virtual EBugsnagBreadcrumbType GetType() const = 0;

	virtual void SetType(EBugsnagBreadcrumbType) = 0;

	/**
	 * Diagnostic data relating to the breadcrumb.
	 */
	virtual TSharedPtr<FJsonObject> GetMetadata() const = 0;

	virtual void SetMetadata(const TSharedPtr<FJsonObject>&) = 0;
};
