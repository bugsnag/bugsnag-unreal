#pragma once

#include "Interfaces/PlatformBugsnag.h"

class FAndroidPlatformBugsnag : public IPlatformBugsnag
{
public:
	void Start(const TSharedPtr<FBugsnagConfiguration>& Configuration) override;

	void Notify(const FString& ErrorClass, const FString& Message) override;

	void Notify(const FString& ErrorClass, const FString& Message, const FBugsnagOnErrorCallback& Callback) override;

	void Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace) override;

	void Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace,
		const FBugsnagOnErrorCallback& Callback) override;

	const FString GetContext() override;

	void SetContext(const FString& Context) override;

	const TSharedPtr<FBugsnagUser> GetUser() override;

	void SetUser(const FString& Id, const FString& Email, const FString& Name) override;

	void AddMetadata(const FString& Section, const TSharedPtr<FJsonObject>& Metadata) override;

	void AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value) override;

	TSharedPtr<FJsonObject> GetMetadata(const FString& Section) override;

	TSharedPtr<FJsonValue> GetMetadata(const FString& Section, const FString& Key) override;

	void ClearMetadata(const FString& Section) override;

	void ClearMetadata(const FString& Section, const FString& Key) override;

	void LeaveBreadcrumb(const FString& Message, const TSharedPtr<FJsonObject>& Metadata, EBugsnagBreadcrumbType Type) override;

	TArray<TSharedPtr<const class IBugsnagBreadcrumb>> GetBreadcrumbs() override;

	void MarkLaunchCompleted() override;

	TSharedPtr<FBugsnagLastRunInfo> GetLastRunInfo() override;

	void StartSession() override;

	void PauseSession() override;

	bool ResumeSession() override;

	void AddOnBreadcrumb(const FBugsnagOnBreadcrumbCallback& Callback) override;

	void AddOnError(const FBugsnagOnErrorCallback& Callback) override;

	void AddOnSession(const FBugsnagOnSessionCallback& Callback) override;
};

DECLARE_PLATFORM_BUGSNAG(FAndroidPlatformBugsnag)
