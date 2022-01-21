// Copyright 2021 Bugsnag. All Rights Reserved.

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
#include "WrappedFeatureFlagStore.h"
#include "WrappedMetadataStore.h"
#include "WrappedThread.h"

#import <Bugsnag/BugsnagEvent.h>
#import <Bugsnag/BugsnagUser.h>

class BUGSNAG_API FWrappedEvent : public IBugsnagEvent, public FWrappedFeatureFlagStore, public FWrappedMetadataStore
{
public:
	static TSharedRef<FWrappedEvent> From(BugsnagEvent* CocoaEvent)
	{
		return MakeShared<FWrappedEvent>(CocoaEvent);
	}

	FWrappedEvent(BugsnagEvent* CocoaEvent)
		: FWrappedMetadataStore(CocoaEvent)
		, CocoaEvent(CocoaEvent)
	{
	}

	// apiKey?: string;

	const TOptional<FString> GetApiKey() const override
	{
		return OptionalFromNSString(CocoaEvent.apiKey);
	}

	void SetApiKey(const TOptional<FString>& Value) override
	{
		CocoaEvent.apiKey = NSStringFromOptional(Value);
	}

	// context?: Context;

	const TOptional<FString> GetContext() const override
	{
		return OptionalFromNSString(CocoaEvent.context);
	}

	void SetContext(const TOptional<FString>& Value) override
	{
		CocoaEvent.context = NSStringFromOptional(Value);
	}

	// groupingHash?: string = null;

	const TOptional<FString> GetGroupingHash() const override
	{
		return OptionalFromNSString(CocoaEvent.groupingHash);
	}

	void SetGroupingHash(const TOptional<FString>& Value) override
	{
		CocoaEvent.groupingHash = NSStringFromOptional(Value);
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
			OptionalFromNSString(CocoaEvent.user.id),
			OptionalFromNSString(CocoaEvent.user.name),
			OptionalFromNSString(CocoaEvent.user.email));
	}

	// setUser(id: String?, email: String?, name: String?): void;

	void SetUser(const TOptional<FString>& Id, const TOptional<FString>& Email, const TOptional<FString>& Name) override
	{
		[CocoaEvent setUser:NSStringFromOptional(Id)
				  withEmail:NSStringFromOptional(Email)
					andName:NSStringFromOptional(Name)];
	}

private:
	BugsnagEvent* CocoaEvent;
};
