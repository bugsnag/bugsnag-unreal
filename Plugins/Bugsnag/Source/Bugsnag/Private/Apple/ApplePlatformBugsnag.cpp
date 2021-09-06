#include "ApplePlatformBugsnag.h"

#import <Bugsnag/Bugsnag.h>

DEFINE_PLATFORM_BUGSNAG(FApplePlatformBugsnag);

void FApplePlatformBugsnag::Start(const TSharedPtr<FBugsnagConfiguration>& Configuration)
{
}

void FApplePlatformBugsnag::Notify(const FString& ErrorClass, const FString& Message)
{
}

void FApplePlatformBugsnag::Notify(const FString& ErrorClass, const FString& Message, const FBugsnagOnErrorCallback& Callback)
{
}

void FApplePlatformBugsnag::Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace)
{
}

void FApplePlatformBugsnag::Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace,
	const FBugsnagOnErrorCallback& Callback)
{
}

const FString FApplePlatformBugsnag::GetContext()
{
	return TEXT("");
}

void FApplePlatformBugsnag::SetContext(const FString& Context)
{
}

const TSharedPtr<FBugsnagUser> FApplePlatformBugsnag::GetUser()
{
	return nullptr;
}

void FApplePlatformBugsnag::SetUser(const FString& Id, const FString& Email, const FString& Name)
{
}

void FApplePlatformBugsnag::AddMetadata(const FString& Section, const TSharedPtr<FJsonObject>& Metadata)
{
}

void FApplePlatformBugsnag::AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value)
{
}

TSharedPtr<FJsonObject> FApplePlatformBugsnag::GetMetadata(const FString& Section)
{
	return nullptr;
}

TSharedPtr<FJsonValue> FApplePlatformBugsnag::GetMetadata(const FString& Section, const FString& Key)
{
	return nullptr;
}

void FApplePlatformBugsnag::ClearMetadata(const FString& Section)
{
}

void FApplePlatformBugsnag::ClearMetadata(const FString& Section, const FString& Key)
{
}

void FApplePlatformBugsnag::LeaveBreadcrumb(const FString& Message, const TSharedPtr<FJsonObject>& Metadata, EBugsnagBreadcrumbType Type)
{
}

TArray<TSharedPtr<const class IBugsnagBreadcrumb>> FApplePlatformBugsnag::GetBreadcrumbs()
{
	return {};
}

void FApplePlatformBugsnag::MarkLaunchCompleted()
{
}

TSharedPtr<FBugsnagLastRunInfo> FApplePlatformBugsnag::GetLastRunInfo()
{
	return nullptr;
}

void FApplePlatformBugsnag::StartSession()
{
}

void FApplePlatformBugsnag::PauseSession()
{
}

bool FApplePlatformBugsnag::ResumeSession()
{
	return false;
}

void FApplePlatformBugsnag::AddOnBreadcrumb(const FBugsnagOnBreadcrumbCallback& Callback)
{
}

void FApplePlatformBugsnag::AddOnError(const FBugsnagOnErrorCallback& Callback)
{
}

void FApplePlatformBugsnag::AddOnSession(const FBugsnagOnSessionCallback& Callback)
{
}
