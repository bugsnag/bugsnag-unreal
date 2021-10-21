#include "BugsnagFunctionLibrary.h"

#include "BugsnagConstants.h"
#include "PlatformBugsnag.h"

#include "Engine/Engine.h"
#include "Misc/CoreDelegates.h"

#include COMPILED_PLATFORM_HEADER(PlatformStackWalk.h)

void UBugsnagFunctionLibrary::Start(const FString& ApiKey)
{
	TSharedPtr<FBugsnagConfiguration> Configuration(new FBugsnagConfiguration(ApiKey));
	Start(Configuration);
}

static TSharedRef<FJsonObject> MakeJsonObject(const FString& Key, const FString& Value)
{
	TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetStringField(Key, Value);
	return JsonObject;
}

void UBugsnagFunctionLibrary::Start(const TSharedPtr<FBugsnagConfiguration>& Configuration)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.Start(Configuration);

	FCoreUObjectDelegates::PreLoadMap.AddLambda([](const FString& MapUrl)
		{
			UE_LOG(LogBugsnag, Log, TEXT("FCoreUObjectDelegates::PreLoadMap %s"), *MapUrl);
			GPlatformBugsnag.LeaveBreadcrumb(BugsnagBreadcrumbMessages::MapLoading,
				MakeJsonObject(BugsnagConstants::Url, MapUrl), EBugsnagBreadcrumbType::Navigation);
			GPlatformBugsnag.AddMetadata(BugsnagConstants::UnrealEngine, BugsnagConstants::MapUrl,
				MakeShared<FJsonValueString>(MapUrl));
		});

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddLambda([](UWorld* World)
		{
			FString MapUrl = World->URL.Map;
			UE_LOG(LogBugsnag, Log, TEXT("FCoreUObjectDelegates::PostLoadMapWithWorld %s"), *MapUrl);
			GPlatformBugsnag.LeaveBreadcrumb(BugsnagBreadcrumbMessages::MapLoaded,
				MakeJsonObject(BugsnagConstants::Url, MapUrl), EBugsnagBreadcrumbType::Navigation);
		});

	FCoreDelegates::GameStateClassChanged.AddLambda([](const FString& InGameStateName)
		{
			UE_LOG(LogBugsnag, Log, TEXT("FCoreDelegates::GameStateClassChanged %s"), *InGameStateName);
			GPlatformBugsnag.LeaveBreadcrumb(BugsnagBreadcrumbMessages::GameStateChanged,
				MakeJsonObject(BugsnagConstants::Name, InGameStateName), EBugsnagBreadcrumbType::State);
			GPlatformBugsnag.AddMetadata(BugsnagConstants::UnrealEngine, BugsnagConstants::GameStateName,
				MakeShared<FJsonValueString>(InGameStateName));
		});

	FCoreDelegates::UserActivityStringChanged.AddLambda([](const FString& InUserActivity)
		{
			UE_LOG(LogBugsnag, Log, TEXT("FCoreDelegates::UserActivityStringChanged %s"), *InUserActivity);
			GPlatformBugsnag.LeaveBreadcrumb(BugsnagBreadcrumbMessages::UserActivityChanged,
				MakeJsonObject(BugsnagConstants::Activity, InUserActivity), EBugsnagBreadcrumbType::User);
			GPlatformBugsnag.AddMetadata(BugsnagConstants::UnrealEngine, BugsnagConstants::UserActivity,
				MakeShared<FJsonValueString>(InUserActivity));
		});
#else
	UE_LOG(LogBugsnag, Warning, TEXT("Bugsnag is not implemented on this platform"));
#endif
}

void UBugsnagFunctionLibrary::Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace,
	const FBugsnagOnErrorCallback& Callback)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.Notify(ErrorClass, Message, StackTrace, Callback);
#endif
}

FORCENOINLINE TArray<uint64> UBugsnagFunctionLibrary::CaptureStackTrace()
{
	static const int MAX_DEPTH = 100;
	uint64 Buffer[MAX_DEPTH];
	FMemory::Memzero(Buffer);

	const uint32 Depth = FPlatformStackWalk::CaptureStackBackTrace(Buffer, MAX_DEPTH);

// Skip the correct number of frames to ensure Shipping builds group correctly
#if PLATFORM_IOS
	const uint32 IgnoreCount = 1;
#elif PLATFORM_ANDROID
	const uint32 IgnoreCount = 2;
#else
	// Calibrate this function when adding a new platform
	const uint32 IgnoreCount = 0;
#endif

	return TArray<uint64>(Buffer + IgnoreCount, Depth - IgnoreCount);
}

const FString UBugsnagFunctionLibrary::GetContext()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	TSharedPtr<FString> Context = GPlatformBugsnag.GetContext();
	return Context.IsValid() ? *Context : TEXT("");
#else
	return TEXT("");
#endif
}

void UBugsnagFunctionLibrary::SetContext(const FString& Context)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	if (Context.IsEmpty())
	{
		GPlatformBugsnag.SetContext(nullptr);
	}
	else
	{
		GPlatformBugsnag.SetContext(MakeShared<FString>(Context));
	}
#endif
}

const FBugsnagUser UBugsnagFunctionLibrary::GetUser()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	return GPlatformBugsnag.GetUser();
#else
	return FBugsnagUser();
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

void UBugsnagFunctionLibrary::AddOnBreadcrumb(FBugsnagOnBreadcrumbCallback Callback)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.AddOnBreadcrumb(Callback);
#endif
}

void UBugsnagFunctionLibrary::AddOnSendError(FBugsnagOnErrorCallback Callback)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.AddOnSendError(Callback);
#endif
}

void UBugsnagFunctionLibrary::AddOnSession(FBugsnagOnSessionCallback Callback)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.AddOnSession(Callback);
#endif
}
