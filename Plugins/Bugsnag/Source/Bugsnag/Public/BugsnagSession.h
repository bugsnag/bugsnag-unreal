#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"

#include "BugsnagApp.h"
#include "BugsnagDevice.h"
#include "BugsnagUser.h"

class BUGSNAG_API IBugsnagSession
{
public:
	virtual const FString GetId() const = 0;

	virtual void SetId(const FString&) = 0;

	virtual const FDateTime GetStartedAt() const = 0;

	virtual void SetStartedAt(const FDateTime&) = 0;

	virtual TSharedPtr<IBugsnagApp> GetApp() = 0;

	virtual TSharedPtr<IBugsnagDevice> GetDevice() = 0;

	virtual const FBugsnagUser GetUser() const = 0;

	virtual void SetUser(const FString& Id = TEXT(""), const FString& Email = TEXT(""), const FString& Name = TEXT("")) = 0;
};
