// Copyright 2021 Bugsnag. All Rights Reserved.

#include "BugsnagFunctionLibrary.h"

#include "BugsnagConstants.h"
#include "LogBugsnag.h"
#include "PlatformBugsnag.h"

#include "Engine/Engine.h"
#include "GameFramework/GameStateBase.h"
#include "HAL/PlatformProperties.h"
#include "Misc/CoreDelegates.h"
#include "Logging/LogMacros.h"

#if PLATFORM_ANDROID
#import <mutex>
#endif

#include COMPILED_PLATFORM_HEADER(PlatformStackWalk.h)

#define LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM()                                          \
	UE_LOG(LogBugsnag, Log,                                                             \
		TEXT("UBugsnagFunctionLibrary::%s(): Bugsnag is not supported on platform %s"), \
		*FString(__func__), *FString(FPlatformProperties::PlatformName()))

void UBugsnagFunctionLibrary::Start(const FString& ApiKey)
{
	TSharedPtr<FBugsnagConfiguration> Configuration = FBugsnagConfiguration::Load();
	if (Configuration.IsValid())
	{
		Configuration->SetApiKey(ApiKey);
	}
	else
	{
		UE_LOG(LogBugsnag, Error, TEXT("Could not load configuration from DefaultEngine.ini"));
		Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
	}
	Start(Configuration.ToSharedRef());
}

#if PLATFORM_IMPLEMENTS_BUGSNAG

static TSharedRef<FJsonObject> MakeJsonObject(const FString& Key, const FString& Value)
{
	TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetStringField(Key, Value);
	return JsonObject;
}

static void GameStateChanged(const FString& InGameStateName)
{
	TSharedPtr<FJsonValue> MetadataValue = GPlatformBugsnag.GetMetadata(BugsnagConstants::UnrealEngine, BugsnagConstants::GameStateName);
	if (MetadataValue.IsValid() && MetadataValue->AsString() == InGameStateName)
	{
		return;
	}
	GPlatformBugsnag.LeaveBreadcrumb(BugsnagBreadcrumbMessages::GameStateChanged,
		MakeJsonObject(BugsnagConstants::Name, InGameStateName), EBugsnagBreadcrumbType::State);
	GPlatformBugsnag.AddMetadata(BugsnagConstants::UnrealEngine, BugsnagConstants::GameStateName,
		MakeShared<FJsonValueString>(InGameStateName));
};

#endif

void UBugsnagFunctionLibrary::Start(const TSharedRef<FBugsnagConfiguration>& Configuration)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG

#if UE_EDITOR
	if (!FApp::IsGame())
	{
		UE_LOG(LogBugsnag, Log, TEXT("Automatic session tracking is disabled in the Editor"));
		Configuration->SetAutoTrackSessions(false);
	}
	else
	{
		// The Editor may have been launched in game mode, e.g. `UE4Editor <uproject> -game`
	}
#endif

	GPlatformBugsnag.Start(Configuration);

	static FString MapUrl;

	FCoreUObjectDelegates::PreLoadMap.AddLambda([](const FString& InMapUrl)
		{
			MapUrl = InMapUrl;
			UE_LOG(LogBugsnag, Log, TEXT("FCoreUObjectDelegates::PreLoadMap %s"), *MapUrl);
			GPlatformBugsnag.LeaveBreadcrumb(BugsnagBreadcrumbMessages::MapLoading,
				MakeJsonObject(BugsnagConstants::Url, MapUrl), EBugsnagBreadcrumbType::Navigation);
		});

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddLambda([](UWorld* World)
		{
			UE_LOG(LogBugsnag, Log, TEXT("FCoreUObjectDelegates::PostLoadMapWithWorld %s"), *MapUrl);
			if (!World)
			{
				GPlatformBugsnag.LeaveBreadcrumb(BugsnagBreadcrumbMessages::MapLoadFailed,
					MakeJsonObject(BugsnagConstants::Url, MapUrl), EBugsnagBreadcrumbType::Navigation);
				return;
			}

			GPlatformBugsnag.LeaveBreadcrumb(BugsnagBreadcrumbMessages::MapLoaded,
				MakeJsonObject(BugsnagConstants::Url, MapUrl), EBugsnagBreadcrumbType::Navigation);

			GPlatformBugsnag.AddMetadata(BugsnagConstants::UnrealEngine, BugsnagConstants::MapUrl,
				MakeShared<FJsonValueString>(MapUrl));

			if (World->GetGameState())
			{
				GameStateChanged(World->GetGameState()->GetClass()->GetName());
			}

			World->GameStateSetEvent.AddLambda([](AGameStateBase* GameState)
				{
					FString GameStateName = GameState->GetClass()->GetName();
					UE_LOG(LogBugsnag, Log, TEXT("UWorld::GameStateSetEvent %s"), *GameStateName);
					GameStateChanged(GameStateName);
				});
		});

	FCoreDelegates::GameStateClassChanged.AddLambda([](const FString& InGameStateName)
		{
			UE_LOG(LogBugsnag, Log, TEXT("FCoreDelegates::GameStateClassChanged %s"), *InGameStateName);
			GameStateChanged(InGameStateName);
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
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

void UBugsnagFunctionLibrary::Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace,
	const FBugsnagOnErrorCallback& Callback)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	UE_LOG(LogBugsnag, Log, TEXT("RICHLOG Notify called with ErrorClass: %s, Message: %s"), *ErrorClass, *Message);

	GPlatformBugsnag.Notify(ErrorClass, Message, StackTrace, Callback);

	UE_LOG(LogBugsnag, Log, TEXT("RICHLOG GPlatformBugsnag.Notify() call completed"));

#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

FORCENOINLINE TArray<uint64> UBugsnagFunctionLibrary::CaptureStackTrace()
{
	static const int MAX_DEPTH = 100;
	uint64 Buffer[MAX_DEPTH];
	FMemory::Memzero(Buffer);

	int32 Depth = 0;
	{
#if PLATFORM_ANDROID
		// CaptureStackBackTrace is not concurrency-safe on Android.
		// See BacktraceCallback in Engine/Source/Runtime/Core/Private/Android/AndroidPlatformStackWalk.cpp
		static std::mutex mutex;
		std::lock_guard<std::mutex> guard(mutex);
#endif
		// Internally, CaptureStackBackTrace is calling functions that return signed int
		// See Unreal source:
		//   - Engine/Source/Runtime/Core/Private/Android/AndroidPlatformStackWalk.cpp
		//   - Engine/Source/Runtime/Core/Private/Apple/ApplePlatformStackWalk.cpp
		Depth = (int32)FPlatformStackWalk::CaptureStackBackTrace(Buffer, MAX_DEPTH);
	}

// Skip the correct number of frames to ensure Shipping builds group correctly
#if PLATFORM_IOS || PLATFORM_MAC
	const int32 IgnoreCount = 1;
#elif PLATFORM_ANDROID
	const int32 IgnoreCount = 2;
#else
	// Calibrate this function when adding a new platform
	const int32 IgnoreCount = 0;
#endif

	const auto AdjustedDepth = Depth - IgnoreCount;
	// Don't trust CaptureStackBackTrace to behave properly.
	if (AdjustedDepth <= 0 || AdjustedDepth > MAX_DEPTH)
	{
		return TArray<uint64>(nullptr, 0);
	}

	return TArray<uint64>(Buffer + IgnoreCount, AdjustedDepth);
}

const FString UBugsnagFunctionLibrary::GetContext()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	return GPlatformBugsnag.GetContext().Get(FString());
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
	return FString();
#endif
}

void UBugsnagFunctionLibrary::SetContext(const FString& Context)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	if (Context.IsEmpty())
	{
		GPlatformBugsnag.SetContext(TOptional<FString>());
	}
	else
	{
		GPlatformBugsnag.SetContext(TOptional<FString>(Context));
	}
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

const FBugsnagUser UBugsnagFunctionLibrary::GetUser()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	return GPlatformBugsnag.GetUser();
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
	return FBugsnagUser();
#endif
}

void UBugsnagFunctionLibrary::SetUser(const FString& Id, const FString& Email, const FString& Name)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.SetUser(Id, Email, Name);
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

void UBugsnagFunctionLibrary::AddFeatureFlag(const FString& Name, const TOptional<FString>& Variant)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.AddFeatureFlag(Name, Variant);
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

void UBugsnagFunctionLibrary::AddFeatureFlags(const TArray<FBugsnagFeatureFlag>& FeatureFlags)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.AddFeatureFlags(FeatureFlags);
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

void UBugsnagFunctionLibrary::ClearFeatureFlag(const FString& Name)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.ClearFeatureFlag(Name);
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

void UBugsnagFunctionLibrary::ClearFeatureFlags()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.ClearFeatureFlags();
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

void UBugsnagFunctionLibrary::AddMetadata(const FString& Section, const TSharedRef<FJsonObject>& Metadata)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.AddMetadata(Section, Metadata);
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

void UBugsnagFunctionLibrary::AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.AddMetadata(Section, Key, Value);
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

TSharedPtr<FJsonObject> UBugsnagFunctionLibrary::GetMetadata(const FString& Section)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	return GPlatformBugsnag.GetMetadata(Section);
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
	return nullptr;
#endif
}

TSharedPtr<FJsonValue> UBugsnagFunctionLibrary::GetMetadata(const FString& Section, const FString& Key)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	return GPlatformBugsnag.GetMetadata(Section, Key);
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
	return nullptr;
#endif
}

void UBugsnagFunctionLibrary::ClearMetadata(const FString& Section)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.ClearMetadata(Section);
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

void UBugsnagFunctionLibrary::ClearMetadata(const FString& Section, const FString& Key)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.ClearMetadata(Section, Key);
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

void UBugsnagFunctionLibrary::LeaveBreadcrumb(const FString& Message, EBugsnagBreadcrumbType Type)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.LeaveBreadcrumb(Message, nullptr, Type);
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

void UBugsnagFunctionLibrary::LeaveBreadcrumb(const FString& Message, const TSharedPtr<FJsonObject>& Metadata, EBugsnagBreadcrumbType Type)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.LeaveBreadcrumb(Message, Metadata, Type);
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

TArray<TSharedRef<const IBugsnagBreadcrumb>> UBugsnagFunctionLibrary::GetBreadcrumbs()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	return GPlatformBugsnag.GetBreadcrumbs();
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
	return {};
#endif
}

void UBugsnagFunctionLibrary::MarkLaunchCompleted()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.MarkLaunchCompleted();
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

TSharedPtr<FBugsnagLastRunInfo> UBugsnagFunctionLibrary::GetLastRunInfo()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	return GPlatformBugsnag.GetLastRunInfo();
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
	return nullptr;
#endif
}

void UBugsnagFunctionLibrary::StartSession()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.StartSession();
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

void UBugsnagFunctionLibrary::PauseSession()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.PauseSession();
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

bool UBugsnagFunctionLibrary::ResumeSession()
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	return GPlatformBugsnag.ResumeSession();
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
	return false;
#endif
}

void UBugsnagFunctionLibrary::AddOnBreadcrumb(FBugsnagOnBreadcrumbCallback Callback)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.AddOnBreadcrumb(Callback);
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}

void UBugsnagFunctionLibrary::AddOnSession(FBugsnagOnSessionCallback Callback)
{
#if PLATFORM_IMPLEMENTS_BUGSNAG
	GPlatformBugsnag.AddOnSession(Callback);
#else
	LOG_NOT_IMPLEMENTED_ON_THIS_PLATFORM();
#endif
}
