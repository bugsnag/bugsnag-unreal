#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "BugsnagConfiguration.h"
#include "BugsnagLastRunInfo.h"
#include "BugsnagUser.h"

#include "BugsnagFunctionLibrary.generated.h"

UCLASS()
class BUGSNAG_API UBugsnagFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Start

	// Start detecting and reporting errors.
	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static void Start(const FString& ApiKey);

	static void Start(const TSharedPtr<FBugsnagConfiguration>& Configuration);

	// Notify

	// Report an error to Bugsnag.
	FORCEINLINE
	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static void Notify(const FString& ErrorClass, const FString& Message)
	{
		Notify(ErrorClass, Message, CaptureStackTrace(), nullptr);
	};

	FORCEINLINE static void Notify(const FString& ErrorClass, const FString& Message, const FBugsnagOnErrorCallback& Callback)
	{
		Notify(ErrorClass, Message, CaptureStackTrace(), Callback);
	};

	static void Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace)
	{
		Notify(ErrorClass, Message, StackTrace, nullptr);
	};

	static void Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace,
		const FBugsnagOnErrorCallback& Callback);

	// Returns the stack trace of the calling thread.
	FORCENOINLINE static TArray<uint64> CaptureStackTrace();

	// Context

	static const FString GetContext();

	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static void SetContext(const FString& Context);

	// User

	static const TSharedPtr<FBugsnagUser> GetUser();

	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static void SetUser(const FString& Id = TEXT(""), const FString& Email = TEXT(""), const FString& Name = TEXT(""));

	// Metadata

	static void AddMetadata(const FString& Section, const TSharedPtr<FJsonObject>& Metadata);

	static void AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value);

	static TSharedPtr<FJsonObject> GetMetadata(const FString& Section);

	static TSharedPtr<FJsonValue> GetMetadata(const FString& Section, const FString& Key);

	static void ClearMetadata(const FString& Section);

	static void ClearMetadata(const FString& Section, const FString& Key);

	// Breadcrumbs

	// Leave a short log statement to help diagnose the events leading to an error.
	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static void LeaveBreadcrumb(const FString& Message, EBugsnagBreadcrumbType Type = EBugsnagBreadcrumbType::Manual);

	static void LeaveBreadcrumb(const FString& Message, const TSharedPtr<FJsonObject>& Metadata,
		EBugsnagBreadcrumbType Type = EBugsnagBreadcrumbType::Manual);

	static TArray<TSharedRef<const IBugsnagBreadcrumb>> GetBreadcrumbs();

	// Crashes On Launch

	// Indicate that your app has finished launching.
	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static void MarkLaunchCompleted();

	static TSharedPtr<FBugsnagLastRunInfo> GetLastRunInfo();

	// Sessions

	// Starts a new session which subsequent handled and unhandled events will be attributed to.
	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static void StartSession();

	// Prevents further events being attributed to the current session until the session is resumed or a new session is started.
	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static void PauseSession();

	// Resumes tracking events against the current session, if it was previously paused. If there is was no previous session, a new session is started.
	// Returns true if a previous session was resumed, false if a new session was started.
	UFUNCTION(BlueprintCallable, Category = "Bugsnag")
	static bool ResumeSession();

	// Filtering

	void AddOnBreadcrumb(const FBugsnagOnBreadcrumbCallback& Callback);

	void AddOnSendError(const FBugsnagOnErrorCallback& Callback);

	void AddOnSession(const FBugsnagOnSessionCallback& Callback);
};
