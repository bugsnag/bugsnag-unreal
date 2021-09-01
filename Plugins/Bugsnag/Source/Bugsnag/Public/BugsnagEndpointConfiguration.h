#pragma once

#include "CoreMinimal.h"
#include "LogBugsnag.h"

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

	FBugsnagEndpointConfiguration(const FString& Notify, const FString& Sessions)
	{
		if (Notify.IsEmpty())
		{
			UE_LOG(LogBugsnag, Warning, TEXT("Invalid configuration. Endpoints.Notify should be a non-empty string, got %s"), *Notify);
		}
		else
		{
			this->Notify = Notify;

			if (Notify != NotifyDefault && Sessions == SessionsDefault)
			{
				UE_LOG(LogBugsnag, Warning, TEXT("Invalid configuration. Endpoints.Notify cannot be set without also setting Endpoints.Sessions. "
												 "Sessions will not be sent to Bugsnag."));
				return;
			}
		}

		if (Sessions.IsEmpty())
		{
			UE_LOG(LogBugsnag, Warning, TEXT("Invalid configuration. Endpoints.Sessions should be a non-empty string, got %s"), *Sessions);
		}
		else
		{
			this->Sessions = Sessions;

			if (Sessions != SessionsDefault && Notify == NotifyDefault)
			{
				UE_LOG(LogBugsnag, Warning, TEXT("Invalid configuration. Endpoints.Sessions cannot be set without also setting Endpoints.Notify. "
												 "Events will not be sent to Bugsnag."));
				this->Notify = TEXT("");
			}
		}
	}

	const FString& GetNotify() const { return Notify; }
	const FString& GetSessions() const { return Sessions; }

private:
	FString Notify;
	FString Sessions;
};
