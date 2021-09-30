#pragma once

#include "CoreMinimal.h"

#include "BugsnagErrorType.h"

class BUGSNAG_API IBugsnagStackframe
{
public:
	// file?: string,

	virtual FString GetFile() const = 0;

	virtual void SetFile(const FString&) = 0;

	// inProject: boolean,

	virtual bool GetInProject() const = 0;

	virtual void SetInProject(bool) = 0;

	// lineNumber?: number,

	virtual TSharedPtr<uint32> GetLineNumber() const = 0;

	virtual void SetLineNumber(const TSharedPtr<uint32>&) = 0;

	// method?: string,

	virtual FString GetMethod() const = 0;

	virtual void SetMethod(const FString&) = 0;

	// type?: ErrorType,

	virtual TSharedPtr<EBugsnagErrorType> GetErrorType() const = 0;

	virtual void SetErrorType(TSharedPtr<EBugsnagErrorType>) = 0;

#if PLATFORM_APPLE

	// machoVMAddress?: number

	virtual TSharedPtr<uint64> GetMachoVmAddress() const = 0;

	virtual void SetMachoVmAddress(const TSharedPtr<uint64>&) = 0;

	// machoFile?: string

	virtual FString GetMachoFile() const = 0;

	virtual void SetMachoFile(const FString&) = 0;

	// symbolAddress?: number

	virtual TSharedPtr<uint64> GetSymbolAddress() const = 0;

	virtual void SetSymbolAddress(const TSharedPtr<uint64>&) = 0;

	// machoLoadAddress?: number

	virtual TSharedPtr<uint64> GetMachoLoadAddress() const = 0;

	virtual void SetMachoLoadAddress(const TSharedPtr<uint64>&) = 0;

	// machoUUID?: string

	virtual FString GetMachoUuid() const = 0;

	virtual void SetMachoUuid(const FString&) = 0;

	// frameAddress?: number

	virtual TSharedPtr<uint64> GetFrameAddress() const = 0;

	virtual void SetFrameAddress(const TSharedPtr<uint64>&) = 0;

	// isPC?: boolean

	virtual TSharedPtr<bool> GetIsPC() const = 0;

	virtual void SetIsPC(const TSharedPtr<bool>&) = 0;

	// isLR?: boolean

	virtual TSharedPtr<bool> GetIsLR() const = 0;

	virtual void SetIsLR(const TSharedPtr<bool>&) = 0;

#endif
};
