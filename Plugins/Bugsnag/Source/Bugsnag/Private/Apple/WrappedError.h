#pragma once

#include "CoreMinimal.h"

#include "AppleBugsnagUtils.h"
#include "BugsnagError.h"
#include "WrappedStackframe.h"

#import <Bugsnag/BugsnagError.h>

class BUGSNAG_API FWrappedError : public IBugsnagError
{
public:
	static TSharedRef<FWrappedError> From(BugsnagError* CocoaError)
	{
		return MakeShared<FWrappedError>(CocoaError);
	}

	FWrappedError(BugsnagError* CocoaError)
		: CocoaError(CocoaError)
	{
	}

	// errorClass: string;

	FString GetErrorClass() const override
	{
		return FStringFromNSString(CocoaError.errorClass);
	}

	void SetErrorClass(const FString& Value) override
	{
		CocoaError.errorClass = NSStringFromFString(Value);
	}

	// errorMessage: string;

	FString GetErrorMessage() const override
	{
		return FStringFromNSString(CocoaError.errorMessage);
	}

	void SetErrorMessage(const FString& Value) override
	{
		CocoaError.errorMessage = NSStringFromFString(Value);
	}

	// type: ErrorType;

	EBugsnagErrorType GetErrorType() const override
	{
		switch (CocoaError.type)
		{
		case BSGErrorTypeC:
			return EBugsnagErrorType::C;
		case BSGErrorTypeCocoa:
			return EBugsnagErrorType::Cocoa;
		default:
			// Note: this is inaccurate
			return EBugsnagErrorType::C;
		}
	}

	void SetErrorType(EBugsnagErrorType Value) override
	{
		switch (Value)
		{
		case EBugsnagErrorType::C:
			CocoaError.type = BSGErrorTypeC;
			break;
		case EBugsnagErrorType::Cocoa:
			CocoaError.type = BSGErrorTypeCocoa;
			break;
		}
	}

	// stacktrace: Stacktrace;

	TArray<TSharedRef<IBugsnagStackframe>> GetStacktrace() const override
	{
		TArray<TSharedRef<IBugsnagStackframe>> Stacktrace;
		for (BugsnagStackframe* Stackframe in CocoaError.stacktrace)
		{
			Stacktrace.Add(FWrappedStackframe::From(Stackframe));
		}
		return Stacktrace;
	}

	void SetStacktrace(const TArray<TSharedRef<IBugsnagStackframe>>& Value) override
	{
		NSMutableArray<BugsnagStackframe*>* Stacktrace = [NSMutableArray arrayWithCapacity:Value.Num()];
		for (const auto& Elem : Value)
		{
			TSharedRef<FWrappedStackframe> WrappedStackframe = StaticCastSharedRef<FWrappedStackframe>(Elem);
			[Stacktrace addObject:WrappedStackframe->CocoaStackframe];
		}
		CocoaError.stacktrace = Stacktrace;
	}

private:
	BugsnagError* CocoaError;
};
