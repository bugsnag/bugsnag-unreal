#pragma once

#include "Interfaces/PlatformBugsnag.h"

class FAndroidPlatformBugsnag : public IPlatformBugsnag
{
public:
	void Start(const TSharedPtr<FBugsnagConfiguration>& Configuration) override;

	void Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace,
		const FBugsnagOnErrorCallback& Callback) override;

	const TOptional<FString> GetContext() override;

	void SetContext(const TOptional<FString>& Context) override;

	const FBugsnagUser GetUser() override;

	void SetUser(const FString& Id, const FString& Email, const FString& Name) override;

	void AddMetadata(const FString& Section, const TSharedRef<FJsonObject>& Metadata) override;

	void AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value) override;

	TSharedPtr<FJsonObject> GetMetadata(const FString& Section) override;

	TSharedPtr<FJsonValue> GetMetadata(const FString& Section, const FString& Key) override;

	void ClearMetadata(const FString& Section) override;

	void ClearMetadata(const FString& Section, const FString& Key) override;

	void LeaveBreadcrumb(const FString& Message, const TSharedPtr<FJsonObject>& Metadata, EBugsnagBreadcrumbType Type) override;

	TArray<TSharedRef<const class IBugsnagBreadcrumb>> GetBreadcrumbs() override;

	void MarkLaunchCompleted() override;

	TSharedPtr<FBugsnagLastRunInfo> GetLastRunInfo() override;

	void StartSession() override;

	void PauseSession() override;

	bool ResumeSession() override;

	void AddOnBreadcrumb(FBugsnagOnBreadcrumbCallback Callback) override
	{
		OnBreadcrumbCallbacks.Add(Callback);
	};

	void AddOnSendError(FBugsnagOnErrorCallback Callback) override;

	void AddOnSession(FBugsnagOnSessionCallback Callback) override
	{
		OnSessionCallbacks.Add(Callback);
	}

	bool RunOnBreadcrumbCallbacks(TSharedRef<IBugsnagBreadcrumb> Crumb)
	{
		return RunCallbacks(OnBreadcrumbCallbacks, Crumb);
	}

	bool RunOnSessionCallbacks(TSharedRef<IBugsnagSession> Session)
	{
		return RunCallbacks(OnSessionCallbacks, Session);
	}

private:
	template <typename T>
	bool RunCallbacks(TArray<TFunction<bool(TSharedRef<T>)>> Callbacks, TSharedRef<T> CallbackArg)
	{
		for (auto& Callback : Callbacks)
		{
			if (!Callback(CallbackArg))
			{
				return false;
			}
		}
		return true;
	}

	TArray<FBugsnagOnBreadcrumbCallback> OnBreadcrumbCallbacks;
	TArray<FBugsnagOnSessionCallback> OnSessionCallbacks;
};

DECLARE_PLATFORM_BUGSNAG(FAndroidPlatformBugsnag)
