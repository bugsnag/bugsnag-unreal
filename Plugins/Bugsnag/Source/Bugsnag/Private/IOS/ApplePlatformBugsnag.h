// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "Interfaces/PlatformBugsnag.h"
#include "WrappedFeatureFlagStore.h"
#include "WrappedMetadataStore.h"

class FApplePlatformBugsnag : public IPlatformBugsnag, public FWrappedFeatureFlagStore, public FWrappedMetadataStore
{
public:
	void Start(const TSharedRef<FBugsnagConfiguration>& Configuration) override;

	void Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace,
		const FBugsnagOnErrorCallback& Callback) override;

	const TOptional<FString> GetContext() override;

	void SetContext(const TOptional<FString>& Context) override;

	const FBugsnagUser GetUser() override;

	void SetUser(const FString& Id, const FString& Email, const FString& Name) override;

	void LeaveBreadcrumb(const FString& Message, const TSharedPtr<FJsonObject>& Metadata, EBugsnagBreadcrumbType Type) override;

	TArray<TSharedRef<const IBugsnagBreadcrumb>> GetBreadcrumbs() override;

	void MarkLaunchCompleted() override;

	TSharedPtr<FBugsnagLastRunInfo> GetLastRunInfo() override;

	void StartSession() override;

	void PauseSession() override;

	bool ResumeSession() override;

	void AddOnBreadcrumb(FBugsnagOnBreadcrumbCallback Callback) override;

	void AddOnSession(FBugsnagOnSessionCallback Callback) override;
};

DECLARE_PLATFORM_BUGSNAG(FApplePlatformBugsnag)
