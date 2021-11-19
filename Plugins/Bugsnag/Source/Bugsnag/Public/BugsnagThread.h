// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "BugsnagErrorType.h"
#include "BugsnagStackframe.h"

/**
 * Information about a thread.
 */
class BUGSNAG_API IBugsnagThread
{
public:
	/**
	 * The unique ID of the thread.
	 */
	virtual FString GetId() const = 0;

	virtual void SetId(const FString&) = 0;

	/**
	 * The name of the thread.
	 *
	 * This is not automatically captured for all error types or platforms.
	 */
	virtual FString GetName() const = 0;

	virtual void SetName(const FString&) = 0;

	/**
	 * True is this was the thread that caused the error.
	 */
	virtual bool GetErrorReportingThread() const = 0;

	/**
	 * The type of thread based on the originating platform (intended for internal use only.)
	 */
	virtual EBugsnagErrorType GetErrorType() const = 0;

	/**
	 * This thread's stack trace.
	 */
	virtual TArray<TSharedRef<IBugsnagStackframe>> GetStacktrace() const = 0;

	virtual void SetStacktrace(const TArray<TSharedRef<IBugsnagStackframe>>&) = 0;
};
