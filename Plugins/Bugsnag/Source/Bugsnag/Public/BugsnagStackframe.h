#pragma once

#include "CoreMinimal.h"

#include "BugsnagErrorType.h"

class BUGSNAG_API IBugsnagStackframe
{
public:
	// file?: string,

	virtual TSharedPtr<FString> GetFile() const = 0;

	virtual void SetFile(const TSharedPtr<FString>&) = 0;

	// inProject: boolean,

	virtual bool GetInProject() const = 0;

	virtual void SetInProject(bool) = 0;

	// lineNumber?: number,

	virtual TOptional<int32> GetLineNumber() const = 0;

	virtual void SetLineNumber(const TOptional<int32>&) = 0;

	// method?: string,

	virtual TSharedPtr<FString> GetMethod() const = 0;

	virtual void SetMethod(const TSharedPtr<FString>&) = 0;

	// type?: ErrorType,

	virtual TOptional<EBugsnagErrorType> GetType() const = 0;

#if PLATFORM_APPLE

	// machoVMAddress?: number

	virtual TOptional<uint64> GetMachoVmAddress() const = 0;

	virtual void SetMachoVmAddress(const TOptional<uint64>&) = 0;

	// machoFile?: string

	virtual TSharedPtr<FString> GetMachoFile() const = 0;

	virtual void SetMachoFile(const TSharedPtr<FString>&) = 0;

	// symbolAddress?: number

	virtual TOptional<uint64> GetSymbolAddress() const = 0;

	virtual void SetSymbolAddress(const TOptional<uint64>&) = 0;

	// machoLoadAddress?: number

	virtual TOptional<uint64> GetMachoLoadAddress() const = 0;

	virtual void SetMachoLoadAddress(const TOptional<uint64>&) = 0;

	// machoUUID?: string

	virtual TSharedPtr<FString> GetMachoUuid() const = 0;

	virtual void SetMachoUuid(const TSharedPtr<FString>&) = 0;

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
