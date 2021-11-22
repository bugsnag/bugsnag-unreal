// Copyright 2021 Bugsnag. All Rights Reserved.

#include "BugsnagEndpointConfiguration.h"
#include "LogBugsnag.h"

const FString FBugsnagEndpointConfiguration::NotifyDefault = TEXT("https://notify.bugsnag.com");
const FString FBugsnagEndpointConfiguration::SessionsDefault = TEXT("https://sessions.bugsnag.com");

FBugsnagEndpointConfiguration::FBugsnagEndpointConfiguration(const FString& Notify, const FString& Sessions)
{
	if (Notify.IsEmpty())
	{
		UE_LOG(LogBugsnag, Warning,
			TEXT("Invalid configuration. Endpoints.Notify should be a non-empty string, got %s"), *Notify);
	}
	else
	{
		this->Notify = Notify;

		if (Notify != NotifyDefault && Sessions == SessionsDefault)
		{
			UE_LOG(LogBugsnag, Warning,
				TEXT("Invalid configuration. Endpoints.Notify cannot be set without also setting Endpoints.Sessions. "
					 "Sessions will not be sent to Bugsnag."));
			return;
		}
	}

	if (Sessions.IsEmpty())
	{
		UE_LOG(LogBugsnag, Warning,
			TEXT("Invalid configuration. Endpoints.Sessions should be a non-empty string, got %s"), *Sessions);
	}
	else
	{
		this->Sessions = Sessions;

		if (Sessions != SessionsDefault && Notify == NotifyDefault)
		{
			UE_LOG(LogBugsnag, Warning,
				TEXT("Invalid configuration. Endpoints.Sessions cannot be set without also setting Endpoints.Notify. "
					 "Events will not be sent to Bugsnag."));
			this->Notify = TEXT("");
		}
	}
}
