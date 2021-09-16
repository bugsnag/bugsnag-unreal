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
