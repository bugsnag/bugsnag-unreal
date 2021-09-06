#include "BugsnagFunctionLibrary.h"

void UBugsnagFunctionLibrary::Start(const FString& ApiKey)
{
	TSharedPtr<FBugsnagConfiguration> Configuration(new FBugsnagConfiguration(ApiKey));
	Start(Configuration);
}

void UBugsnagFunctionLibrary::Start(const TSharedPtr<FBugsnagConfiguration>& Configuration)
{
}

void UBugsnagFunctionLibrary::Notify(const FString& ErrorClass, const FString& Message)
{
}

void UBugsnagFunctionLibrary::Notify(const FString& ErrorClass, const FString& Message, const FBugsnagOnErrorCallback& Callback)
{
}

void UBugsnagFunctionLibrary::Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace)
{
}

void UBugsnagFunctionLibrary::Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace,
	const FBugsnagOnErrorCallback& Callback)
{
}

TArray<uint64> UBugsnagFunctionLibrary::CaptureStackTrace()
{
	static const int MAX_DEPTH = 100;
	uint64 Buffer[MAX_DEPTH];
	FMemory::Memzero(Buffer);

	const uint32 Depth = FPlatformStackWalk::CaptureStackBackTrace(Buffer, MAX_DEPTH);

	const uint32 IgnoreCount = 2; // Ignore this function and `FPlatformStackWalk::CaptureStackBackTrace`

	return TArray<uint64>(Buffer + IgnoreCount, Depth - IgnoreCount);
}

const FString UBugsnagFunctionLibrary::GetContext()
{
	return TEXT("");
}

void UBugsnagFunctionLibrary::SetContext(const FString& Context)
{
}

const TSharedPtr<FBugsnagUser> UBugsnagFunctionLibrary::GetUser()
{
	return nullptr;
}

void UBugsnagFunctionLibrary::SetUser(const FString& Id, const FString& Email, const FString& Name)
{
}

void UBugsnagFunctionLibrary::AddMetadata(const FString& Section, const TSharedPtr<FJsonObject>& Metadata)
{
}

void UBugsnagFunctionLibrary::AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value)
{
}

TSharedPtr<FJsonObject> UBugsnagFunctionLibrary::GetMetadata(const FString& Section)
{
	return nullptr;
}

TSharedPtr<FJsonValue> UBugsnagFunctionLibrary::GetMetadata(const FString& Section, const FString& Key)
{
	return nullptr;
}

void UBugsnagFunctionLibrary::ClearMetadata(const FString& Section)
{
}

void UBugsnagFunctionLibrary::ClearMetadata(const FString& Section, const FString& Key)
{
}

void UBugsnagFunctionLibrary::LeaveBreadcrumb(const FString& Message, EBugsnagBreadcrumbType Type)
{
}

void UBugsnagFunctionLibrary::LeaveBreadcrumb(const FString& Message, const TSharedPtr<FJsonObject>& Metadata, EBugsnagBreadcrumbType Type)
{
}

TArray<const TSharedPtr<const class IBugsnagBreadcrumb>> UBugsnagFunctionLibrary::GetBreadcrumbs()
{
	return {};
}

void UBugsnagFunctionLibrary::MarkLaunchCompleted()
{
}

TSharedPtr<FBugsnagLastRunInfo> UBugsnagFunctionLibrary::GetLastRunInfo()
{
	return nullptr;
}

void UBugsnagFunctionLibrary::StartSession()
{
}

void UBugsnagFunctionLibrary::PauseSession()
{
}

bool UBugsnagFunctionLibrary::ResumeSession()
{
	return false;
}

void UBugsnagFunctionLibrary::AddOnBreadcrumb(const FBugsnagOnBreadcrumbCallback& Callback)
{
}

void UBugsnagFunctionLibrary::AddOnError(const FBugsnagOnErrorCallback& Callback)
{
}

void UBugsnagFunctionLibrary::AddOnSession(const FBugsnagOnSessionCallback& Callback)
{
}
