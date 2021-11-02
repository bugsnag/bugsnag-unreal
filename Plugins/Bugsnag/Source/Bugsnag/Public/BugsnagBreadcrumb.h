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
	Manual = 1 << 0,
	Error = 1 << 1,
	Log = 1 << 2,
	Navigation = 1 << 3,
	Process = 1 << 4,
	Request = 1 << 5,
	State = 1 << 6,
	User = 1 << 7,
	All = Manual | Error | Log | Navigation | Process | Request | State | User
};
ENUM_CLASS_FLAGS(EBugsnagEnabledBreadcrumbTypes)

class BUGSNAG_API IBugsnagBreadcrumb
{
public:
	virtual const FDateTime GetTimestamp() const = 0;

	virtual const FString GetMessage() const = 0;

	virtual void SetMessage(const FString&) = 0;

	virtual EBugsnagBreadcrumbType GetType() const = 0;

	virtual void SetType(EBugsnagBreadcrumbType) = 0;

	virtual TSharedPtr<FJsonObject> GetMetadata() const = 0;

	virtual void SetMetadata(const TSharedPtr<FJsonObject>&) = 0;
};
