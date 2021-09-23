#pragma once

#include "CoreMinimal.h"

#include "BugsnagErrorType.h"
#include "BugsnagStackframe.h"

class BUGSNAG_API IBugsnagThread
{
public:
	// id: string;

	virtual FString GetId() const = 0;

	virtual void SetId(const FString&) = 0;

	// name: string;

	virtual FString GetName() const = 0;

	virtual void SetName(const FString&) = 0;

	// readonly errorReportingThread: boolean;

	virtual bool GetErrorReportingThread() const = 0;

	// type: ErrorType;

	virtual EBugsnagErrorType GetErrorType() const = 0;

	virtual void SetErrorType(EBugsnagErrorType) = 0;

	// stacktrace: Stacktrace;

	virtual TArray<TSharedRef<IBugsnagStackframe>> GetStacktrace() const = 0;

	virtual void SetStacktrace(const TArray<TSharedRef<IBugsnagStackframe>>&) = 0;
};
