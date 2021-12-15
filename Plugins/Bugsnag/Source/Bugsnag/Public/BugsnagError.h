// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "BugsnagErrorType.h"
#include "BugsnagStackframe.h"

/**
 * Information about an error detected by Bugsnag or reported via Notify().
 *
 * Can be amended from an OnError callback if necessary.
 */
class BUGSNAG_API IBugsnagError
{
public:
	/**
	 * The class of the error generating the report.
	 */
	virtual FString GetErrorClass() const = 0;

	virtual void SetErrorClass(const FString&) = 0;

	/**
	 * The message of or reason for the error generating the report.
	 */
	virtual FString GetErrorMessage() const = 0;

	virtual void SetErrorMessage(const FString&) = 0;

	/**
	 * The type of error based on the originating platform (intended for internal use only.)
	 */
	virtual EBugsnagErrorType GetErrorType() const = 0;

	/**
	 * The stack trace at the time the error ocurred.
	 */
	virtual TArray<TSharedRef<IBugsnagStackframe>> GetStacktrace() const = 0;

	virtual void SetStacktrace(const TArray<TSharedRef<IBugsnagStackframe>>&) = 0;
};
