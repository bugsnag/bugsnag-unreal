#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

#include "BugsnagConfiguration.h"
#include "BugsnagLastRunInfo.h"
#include "BugsnagUser.h"

class IPlatformBugsnag
{
public:
	virtual void Start(const TSharedPtr<FBugsnagConfiguration>& Configuration) = 0;

	virtual void Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace,
		const FBugsnagOnErrorCallback& Callback) = 0;

	virtual const FString GetContext() = 0;

	virtual void SetContext(const FString& Context) = 0;

	virtual const TSharedPtr<FBugsnagUser> GetUser() = 0;

	virtual void SetUser(const FString& Id, const FString& Email, const FString& Name) = 0;

	virtual void AddMetadata(const FString& Section, const TSharedPtr<FJsonObject>& Metadata) = 0;

	virtual void AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value) = 0;

	virtual TSharedPtr<FJsonObject> GetMetadata(const FString& Section) = 0;

	virtual TSharedPtr<FJsonValue> GetMetadata(const FString& Section, const FString& Key) = 0;

	virtual void ClearMetadata(const FString& Section) = 0;

	virtual void ClearMetadata(const FString& Section, const FString& Key) = 0;

	virtual void LeaveBreadcrumb(const FString& Message, const TSharedPtr<FJsonObject>& Metadata, EBugsnagBreadcrumbType Type) = 0;

	virtual TArray<TSharedRef<const IBugsnagBreadcrumb>> GetBreadcrumbs() = 0;

	virtual void MarkLaunchCompleted() = 0;

	virtual TSharedPtr<FBugsnagLastRunInfo> GetLastRunInfo() = 0;

	virtual void StartSession() = 0;

	virtual void PauseSession() = 0;

	virtual bool ResumeSession() = 0;

	virtual void AddOnBreadcrumb(const FBugsnagOnBreadcrumbCallback& Callback) = 0;

	virtual void AddOnSendError(const FBugsnagOnErrorCallback& Callback) = 0;

	virtual void AddOnSession(const FBugsnagOnSessionCallback& Callback) = 0;
};

#define DECLARE_PLATFORM_BUGSNAG(CLASS) \
	typedef CLASS FPlatformBugsnag;     \
	extern FPlatformBugsnag GPlatformBugsnag;

#define DEFINE_PLATFORM_BUGSNAG(CLASS) \
	CLASS GPlatformBugsnag;
