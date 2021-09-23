#pragma once

#include "CoreMinimal.h"

#include "BugsnagErrorType.h"

class BUGSNAG_API IBugsnagError
{
public:
	// errorClass: string;

	virtual FString GetErrorClass() const = 0;

	virtual void SetErrorClass(const FString&) = 0;

	// errorMessage: string;

	virtual FString GetErrorMessage() const = 0;

	virtual void SetErrorMessage(const FString&) = 0;

	// type: ErrorType;

	virtual EBugsnagErrorType GetErrorType() const = 0;

	virtual void SetErrorType(EBugsnagErrorType) = 0;
};
