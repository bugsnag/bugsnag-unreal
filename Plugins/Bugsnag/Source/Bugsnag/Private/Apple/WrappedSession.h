#pragma once

#include "AppleBugsnagUtils.h"
#include "BugsnagSession.h"
#include "WrappedApp.h"
#include "WrappedDevice.h"

#import <Bugsnag/BugsnagSession.h>

class FWrappedSession : public IBugsnagSession
{
public:
	static TSharedPtr<FWrappedSession> From(BugsnagSession* CocoaSession)
	{
		return MakeShareable(new FWrappedSession(CocoaSession));
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

	TSharedPtr<IBugsnagApp> GetApp()
	{
		return FWrappedApp::From(CocoaSession.app);
	}

	TSharedPtr<IBugsnagDevice> GetDevice()
	{
		return FWrappedDevice::From(CocoaSession.device);
	}

	const FBugsnagUser GetUser() const
	{
		return FBugsnagUser(
			FStringFromNSString(CocoaSession.user.id),
			FStringFromNSString(CocoaSession.user.email),
			FStringFromNSString(CocoaSession.user.name));
	}

	void SetUser(const FString& Id = TEXT(""), const FString& Email = TEXT(""), const FString& Name = TEXT(""))
	{
		[CocoaSession setUser:NSStringFromFString(Id)
					withEmail:NSStringFromFString(Email)
					  andName:NSStringFromFString(Name)];
	}

private:
	FWrappedSession(BugsnagSession* CocoaSession)
		: CocoaSession(CocoaSession)
	{
	}

	BugsnagSession* CocoaSession;
};
