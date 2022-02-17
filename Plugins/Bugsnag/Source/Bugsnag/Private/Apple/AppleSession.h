// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "AppleApp.h"
#include "AppleBugsnagUtils.h"
#include "AppleDevice.h"
#include "BugsnagSession.h"

#import <Bugsnag/BugsnagSession.h>

class FAppleSession : public IBugsnagSession
{
public:
	static TSharedRef<FAppleSession> From(BugsnagSession* CocoaSession)
	{
		return MakeShared<FAppleSession>(CocoaSession);
	}

	FAppleSession(BugsnagSession* CocoaSession)
		: CocoaSession(CocoaSession)
	{
	}

	const FString GetId() const
	{
		return FStringFromNSString(CocoaSession.id);
	}

	void SetId(const FString& Id)
	{
		CocoaSession.id = NSStringFromFString(Id);
	}

	const FDateTime GetStartedAt() const
	{
		return FDateTimeFromNSDate(CocoaSession.startedAt);
	}

	void SetStartedAt(const FDateTime& StartedAt)
	{
		CocoaSession.startedAt = NSDateFromFDateTime(StartedAt);
	}

	TSharedRef<IBugsnagApp> GetApp()
	{
		return FAppleApp::From(CocoaSession.app);
	}

	TSharedRef<IBugsnagDevice> GetDevice()
	{
		return FAppleDevice::From(CocoaSession.device);
	}

	const FBugsnagUser GetUser() const
	{
		return FBugsnagUser(
			OptionalFromNSString(CocoaSession.user.id),
			OptionalFromNSString(CocoaSession.user.email),
			OptionalFromNSString(CocoaSession.user.name));
	}

	void SetUser(const TOptional<FString>& Id, const TOptional<FString>& Email, const TOptional<FString>& Name)
	{
		[CocoaSession setUser:NSStringFromOptional(Id)
					withEmail:NSStringFromOptional(Email)
					  andName:NSStringFromOptional(Name)];
	}

private:
	BugsnagSession* CocoaSession;
};
