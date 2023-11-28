// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

#include "BugsnagConfiguration.h"
#include "BugsnagLastRunInfo.h"
#include "BugsnagMetadataStore.h"
#include "BugsnagUser.h"

class IPlatformBugsnag : public virtual IBugsnagFeatureFlagStore, public virtual IBugsnagMetadataStore
{
public:
	virtual void Start(const TSharedRef<FBugsnagConfiguration>& Configuration) = 0;

	virtual void Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace,
		const FBugsnagOnErrorCallback& Callback) = 0;

	virtual const TOptional<FString> GetContext() = 0;

	virtual void SetContext(const TOptional<FString>& Context) = 0;

	virtual const FBugsnagUser GetUser() = 0;

	virtual void SetUser(const FString& Id, const FString& Email, const FString& Name) = 0;

	virtual void LeaveBreadcrumb(const FString& Message, const TSharedPtr<FJsonObject>& Metadata, EBugsnagBreadcrumbType Type) = 0;

	virtual TArray<TSharedRef<const IBugsnagBreadcrumb>> GetBreadcrumbs() = 0;

	virtual void MarkLaunchCompleted() = 0;

	virtual TSharedPtr<FBugsnagLastRunInfo> GetLastRunInfo() = 0;

	virtual void StartSession() = 0;

	virtual void PauseSession() = 0;

	virtual bool ResumeSession() = 0;

	virtual void AddOnBreadcrumb(FBugsnagOnBreadcrumbCallback Callback) = 0;

	virtual void AddOnSession(FBugsnagOnSessionCallback Callback) = 0;
};

#define DECLARE_PLATFORM_BUGSNAG(CLASS) \
	typedef CLASS FPlatformBugsnag;     \
	extern FPlatformBugsnag GPlatformBugsnag;

#define DEFINE_PLATFORM_BUGSNAG(CLASS) \
	CLASS GPlatformBugsnag;
