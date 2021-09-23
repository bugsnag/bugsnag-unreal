#include "BugsnagFunctionLibrary.h"
#include "PlatformBugsnag.h"

#include COMPILED_PLATFORM_HEADER(PlatformStackWalk.h)

void UBugsnagFunctionLibrary::Start(const FString& ApiKey)
{
	TSharedPtr<FBugsnagConfiguration> Configuration(new FBugsnagConfiguration(ApiKey));
	Start(Configuration);
}

void UBugsnagFunctionLibrary::Start(const TSharedPtr<FBugsnagConfiguration>& Configuration)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.Start(Configuration);
#else
	UE_LOG(LogBugsnag, Warning, TEXT("Bugsnag is not implemented on this platform"));
#endif
}

void UBugsnagFunctionLibrary::Notify(const FString& ErrorClass, const FString& Message)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.Notify(ErrorClass, Message);
#endif
}

void UBugsnagFunctionLibrary::Notify(const FString& ErrorClass, const FString& Message, const FBugsnagOnErrorCallback& Callback)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.Notify(ErrorClass, Message, Callback);
#endif
}

void UBugsnagFunctionLibrary::Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.Notify(ErrorClass, Message, StackTrace);
#endif
}

void UBugsnagFunctionLibrary::Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace,
	const FBugsnagOnErrorCallback& Callback)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.Notify(ErrorClass, Message, StackTrace, Callback);
#endif
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
#if PLATFORM_IMPLEMENTS_BUGSNAG
	return GPlatformBugsnag.GetContext();
#else
	return TEXT("");
#endif
}

void UBugsnagFunctionLibrary::SetContext(const FString& Context)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.SetContext(Context);
#endif
}

const TSharedPtr<FBugsnagUser> UBugsnagFunctionLibrary::GetUser()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	return GPlatformBugsnag.GetUser();
#else
	return nullptr;
#endif
}

void UBugsnagFunctionLibrary::SetUser(const FString& Id, const FString& Email, const FString& Name)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.SetUser(Id, Email, Name);
#endif
}

void UBugsnagFunctionLibrary::AddMetadata(const FString& Section, const TSharedPtr<FJsonObject>& Metadata)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.AddMetadata(Section, Metadata);
#endif
}

void UBugsnagFunctionLibrary::AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.AddMetadata(Section, Key, Value);
#endif
}

TSharedPtr<FJsonObject> UBugsnagFunctionLibrary::GetMetadata(const FString& Section)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	return GPlatformBugsnag.GetMetadata(Section);
#else
	return nullptr;
#endif
}

TSharedPtr<FJsonValue> UBugsnagFunctionLibrary::GetMetadata(const FString& Section, const FString& Key)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	return GPlatformBugsnag.GetMetadata(Section, Key);
#else
	return nullptr;
#endif
}

void UBugsnagFunctionLibrary::ClearMetadata(const FString& Section)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.ClearMetadata(Section);
#endif
}

void UBugsnagFunctionLibrary::ClearMetadata(const FString& Section, const FString& Key)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.ClearMetadata(Section, Key);
#endif
}

void UBugsnagFunctionLibrary::LeaveBreadcrumb(const FString& Message, EBugsnagBreadcrumbType Type)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.LeaveBreadcrumb(Message, nullptr, Type);
#endif
}

void UBugsnagFunctionLibrary::LeaveBreadcrumb(const FString& Message, const TSharedPtr<FJsonObject>& Metadata, EBugsnagBreadcrumbType Type)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.LeaveBreadcrumb(Message, Metadata, Type);
#endif
}

TArray<TSharedRef<const IBugsnagBreadcrumb>> UBugsnagFunctionLibrary::GetBreadcrumbs()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	return GPlatformBugsnag.GetBreadcrumbs();
#else
	return {};
#endif
}

void UBugsnagFunctionLibrary::MarkLaunchCompleted()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.MarkLaunchCompleted();
#endif
}

TSharedPtr<FBugsnagLastRunInfo> UBugsnagFunctionLibrary::GetLastRunInfo()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	return GPlatformBugsnag.GetLastRunInfo();
#else
	return nullptr;
#endif
}

void UBugsnagFunctionLibrary::StartSession()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.StartSession();
#endif
}

void UBugsnagFunctionLibrary::PauseSession()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.PauseSession();
#endif
}

bool UBugsnagFunctionLibrary::ResumeSession()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	return GPlatformBugsnag.ResumeSession();
#else
	return false;
#endif
}

void UBugsnagFunctionLibrary::AddOnBreadcrumb(const FBugsnagOnBreadcrumbCallback& Callback)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.AddOnBreadcrumb(Callback);
#endif
}

void UBugsnagFunctionLibrary::AddOnError(const FBugsnagOnErrorCallback& Callback)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.AddOnError(Callback);
#endif
}

void UBugsnagFunctionLibrary::AddOnSession(const FBugsnagOnSessionCallback& Callback)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.AddOnSession(Callback);
#endif
}
