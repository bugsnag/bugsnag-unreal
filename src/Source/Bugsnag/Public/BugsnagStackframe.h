// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "BugsnagErrorType.h"

/**
 * Represents a single frame of a stacktrace.
 *
 * Can be amended from an OnError callback if necessary.
 */
class BUGSNAG_API IBugsnagStackframe
{
public:
	/**
	 * The location of the source file corresponding to this stack frame.
	 */
	virtual TOptional<FString> GetFile() const = 0;

	virtual void SetFile(const TOptional<FString>&) = 0;

	/**
	 * Whether this stack frame's code is considered to be in your project for
	 * the purposes of grouping and readability on the Bugsnag dashboard.
	 */
	virtual bool GetInProject() const = 0;

	virtual void SetInProject(bool) = 0;

	/**
	 * The line number within the source file this stack frame refers to.
	 */
	virtual TOptional<int32> GetLineNumber() const = 0;

	virtual void SetLineNumber(const TOptional<int32>&) = 0;

	/**
	 * The name of the function or method that was being executed.
	 *
	 * Note: may be null if debug symbols are missing from the app.
	 */
	virtual TOptional<FString> GetMethod() const = 0;

	virtual void SetMethod(const TOptional<FString>&) = 0;

	/**
	 * The type of the stack frame, if it differs from that of the containing error or event.
	 */
	virtual TOptional<EBugsnagErrorType> GetType() const = 0;

#if PLATFORM_APPLE

	// machoVMAddress?: number

	virtual TOptional<uint64> GetMachoVmAddress() const = 0;

	virtual void SetMachoVmAddress(const TOptional<uint64>&) = 0;

	// machoFile?: string

	virtual TOptional<FString> GetMachoFile() const = 0;

	virtual void SetMachoFile(const TOptional<FString>&) = 0;

	// symbolAddress?: number

	virtual TOptional<uint64> GetSymbolAddress() const = 0;

	virtual void SetSymbolAddress(const TOptional<uint64>&) = 0;

	// machoLoadAddress?: number

	virtual TOptional<uint64> GetMachoLoadAddress() const = 0;

	virtual void SetMachoLoadAddress(const TOptional<uint64>&) = 0;

	// machoUUID?: string

	virtual TOptional<FString> GetMachoUuid() const = 0;

	virtual void SetMachoUuid(const TOptional<FString>&) = 0;

	// frameAddress?: number

	virtual TOptional<uint64> GetFrameAddress() const = 0;

	virtual void SetFrameAddress(const TOptional<uint64>&) = 0;

	// isPC?: boolean

	virtual TOptional<bool> GetIsPC() const = 0;

	virtual void SetIsPC(const TOptional<bool>&) = 0;

	// isLR?: boolean

	virtual TOptional<bool> GetIsLR() const = 0;

	virtual void SetIsLR(const TOptional<bool>&) = 0;

#endif
};
