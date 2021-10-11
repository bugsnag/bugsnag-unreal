#pragma once

#include "CoreMinimal.h"

#include "AppleBugsnagUtils.h"
#include "BugsnagConfiguration.h"
#include "BugsnagEvent.h"
#include "BugsnagUser.h"
#include "WrappedAppWithState.h"
#include "WrappedBreadcrumb.h"
#include "WrappedDeviceWithState.h"
#include "WrappedError.h"
#include "WrappedThread.h"

#import <Bugsnag/BugsnagEvent.h>
#import <Bugsnag/BugsnagUser.h>

class BUGSNAG_API FWrappedEvent : public IBugsnagEvent
{
public:
	static TSharedRef<FWrappedEvent> From(BugsnagEvent* CocoaEvent)
	{
		return MakeShared<FWrappedEvent>(CocoaEvent);
	}

	FWrappedEvent(BugsnagEvent* CocoaEvent)
		: CocoaEvent(CocoaEvent)
	{
	}

	// apiKey?: string;

	const TSharedPtr<FString> GetApiKey() const override
	{
		return FStringPtrFromNSString(CocoaEvent.apiKey);
	}

	void SetApiKey(const TSharedPtr<FString>& Value) override
	{
		CocoaEvent.apiKey = NSStringFromFStringPtr(Value);
	}

	// context?: Context;

	const TSharedPtr<FString> GetContext() const override
	{
		return FStringPtrFromNSString(CocoaEvent.context);
	}

	void SetContext(const TSharedPtr<FString>& Value) override
	{
		CocoaEvent.context = NSStringFromFStringPtr(Value);
	}

	// groupingHash?: string = null;

	const TSharedPtr<FString> GetGroupingHash() const override
	{
		return FStringPtrFromNSString(CocoaEvent.groupingHash);
	}

	void SetGroupingHash(const TSharedPtr<FString>& Value) override
	{
		CocoaEvent.groupingHash = NSStringFromFStringPtr(Value);
	}

	// readonly app: AppWithState;

	const TSharedRef<IBugsnagAppWithState> GetApp() const override
	{
		return FWrappedAppWithState::From(CocoaEvent.app);
	}

	// readonly device: DeviceWithState;

	const TSharedRef<IBugsnagDeviceWithState> GetDevice() const override
	{
		return FWrappedDeviceWithState::From(CocoaEvent.device);
	}

	// readonly breadcrumbs: Breadcrumb[];

	const TArray<TSharedRef<IBugsnagBreadcrumb>> GetBreadcrumbs() const override
	{
		TArray<TSharedRef<IBugsnagBreadcrumb>> Breadcrumbs;
		for (BugsnagBreadcrumb* Breadcrumb in CocoaEvent.breadcrumbs)
		{
			Breadcrumbs.Add(FWrappedBreadcrumb::From(Breadcrumb));
		}
		return Breadcrumbs;
	}

	// readonly errors: Error[];

	const TArray<TSharedRef<IBugsnagError>> GetErrors() const override
	{
		TArray<TSharedRef<IBugsnagError>> Errors;
		for (BugsnagError* CocoaError in CocoaEvent.errors)
		{
			Errors.Add(FWrappedError::From(CocoaError));
		}
		return Errors;
	}

	// readonly threads: Thread[];

	const TArray<TSharedRef<IBugsnagThread>> GetThreads() const override
	{
		TArray<TSharedRef<IBugsnagThread>> Threads;
		for (BugsnagThread* Thread in CocoaEvent.threads)
		{
			Threads.Add(FWrappedThread::From(Thread));
		}
		return Threads;
	}

	// severity: Severity;

	const EBugsnagSeverity GetSeverity() const
	{
		switch (CocoaEvent.severity)
		{
		case BSGSeverityError:
			return EBugsnagSeverity::Error;
		case BSGSeverityWarning:
			return EBugsnagSeverity::Warning;
		case BSGSeverityInfo:
			return EBugsnagSeverity::Info;
		}
	}

	void SetSeverity(EBugsnagSeverity Value)
	{
		switch (Value)
		{
		case EBugsnagSeverity::Error:
			CocoaEvent.severity = BSGSeverityError;
			break;
		case EBugsnagSeverity::Warning:
			CocoaEvent.severity = BSGSeverityWarning;
			break;
		case EBugsnagSeverity::Info:
			CocoaEvent.severity = BSGSeverityInfo;
			break;
		}
	}

	// unhandled: boolean;

	const bool GetUnhandled() const override
	{
		return CocoaEvent.unhandled;
	}

	void SetUnhandled(bool Value) override
	{
		CocoaEvent.unhandled = Value;
	}

	// getUser(): User;

	const FBugsnagUser GetUser() const override
	{
		return FBugsnagUser(
			FStringPtrFromNSString(CocoaEvent.user.id),
			FStringPtrFromNSString(CocoaEvent.user.name),
			FStringPtrFromNSString(CocoaEvent.user.email));
	}

	// setUser(id: String?, email: String?, name: String?): void;

	void SetUser(const TSharedPtr<FString>& Id, const TSharedPtr<FString>& Email, const TSharedPtr<FString>& Name) override
	{
		[CocoaEvent setUser:NSStringFromFStringPtr(Id)
				  withEmail:NSStringFromFStringPtr(Email)
					andName:NSStringFromFStringPtr(Name)];
	}

private:
	BugsnagEvent* CocoaEvent;
};
