#pragma once

#include "CoreMinimal.h"

#include "BugsnagAppWithState.h"
#include "BugsnagBreadcrumb.h"
#include "BugsnagDeviceWithState.h"
#include "BugsnagError.h"
#include "BugsnagMetadataStore.h"
#include "BugsnagThread.h"
#include "BugsnagUser.h"

UENUM()
enum class EBugsnagSeverity : uint8
{
	Info,
	Warning,
	Error,
};

class BUGSNAG_API IBugsnagEvent : public virtual IBugsnagMetadataStore
{
public:
	// apiKey?: string;

	virtual const TSharedPtr<FString> GetApiKey() const = 0;

	virtual void SetApiKey(const TSharedPtr<FString>&) = 0;

	// context?: Context;

	virtual const TSharedPtr<FString> GetContext() const = 0;

	virtual void SetContext(const TSharedPtr<FString>&) = 0;

	// groupingHash?: string = null;

	virtual const TSharedPtr<FString> GetGroupingHash() const = 0;

	virtual void SetGroupingHash(const TSharedPtr<FString>&) = 0;

	// readonly app: AppWithState;

	virtual const TSharedRef<IBugsnagAppWithState> GetApp() const = 0;

	// readonly device: DeviceWithState;

	virtual const TSharedRef<IBugsnagDeviceWithState> GetDevice() const = 0;

	// readonly breadcrumbs: Breadcrumb[];

	virtual const TArray<TSharedRef<IBugsnagBreadcrumb>> GetBreadcrumbs() const = 0;

	// readonly errors: Error[];

	virtual const TArray<TSharedRef<IBugsnagError>> GetErrors() const = 0;

	// readonly threads: Thread[];

	virtual const TArray<TSharedRef<IBugsnagThread>> GetThreads() const = 0;

	// severity: Severity;

	virtual const EBugsnagSeverity GetSeverity() const = 0;

	virtual void SetSeverity(EBugsnagSeverity) = 0;

	// unhandled: boolean;

	virtual const bool GetUnhandled() const = 0;

	virtual void SetUnhandled(bool) = 0;

	// getUser(): User;

	virtual const FBugsnagUser GetUser() const = 0;

	// setUser(id: String?, email: String?, name: String?): void;

	virtual void SetUser(
		const TSharedPtr<FString>& Id = nullptr,
		const TSharedPtr<FString>& Email = nullptr,
		const TSharedPtr<FString>& Name = nullptr) = 0;
};
