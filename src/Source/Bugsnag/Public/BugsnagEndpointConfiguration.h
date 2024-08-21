// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class BUGSNAG_API FBugsnagEndpointConfiguration
{
	static const FString NotifyDefault;
	static const FString SessionsDefault;

public:
	FBugsnagEndpointConfiguration()
		: Notify(NotifyDefault)
		, Sessions(SessionsDefault)
	{
	}

	FBugsnagEndpointConfiguration(const FBugsnagEndpointConfiguration& Endpoints)
		: Notify(Endpoints.Notify)
		, Sessions(Endpoints.Sessions)
	{
	}

	FBugsnagEndpointConfiguration(const FString& Notify, const FString& Sessions);

	const FString& GetNotify() const { return Notify; }
	const FString& GetSessions() const { return Sessions; }

private:
	FString Notify;
	FString Sessions;
};
