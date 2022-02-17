// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "AppleBugsnagUtils.h"
#include "AppleStackframe.h"
#include "BugsnagError.h"

#import <Bugsnag/BugsnagError.h>

class BUGSNAG_API FAppleError : public IBugsnagError
{
public:
	static TSharedRef<FAppleError> From(BugsnagError* CocoaError)
	{
		return MakeShared<FAppleError>(CocoaError);
	}

	FAppleError(BugsnagError* CocoaError)
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

	// stacktrace: Stacktrace;

	TArray<TSharedRef<IBugsnagStackframe>> GetStacktrace() const override
	{
		TArray<TSharedRef<IBugsnagStackframe>> Stacktrace;
		for (BugsnagStackframe* Stackframe in CocoaError.stacktrace)
		{
			Stacktrace.Add(FAppleStackframe::From(Stackframe));
		}
		return Stacktrace;
	}

	void SetStacktrace(const TArray<TSharedRef<IBugsnagStackframe>>& Value) override
	{
		NSMutableArray<BugsnagStackframe*>* Stacktrace = [NSMutableArray arrayWithCapacity:Value.Num()];
		for (const auto& Elem : Value)
		{
			TSharedRef<FAppleStackframe> AppleStackframe = StaticCastSharedRef<FAppleStackframe>(Elem);
			[Stacktrace addObject:AppleStackframe->CocoaStackframe];
		}
		CocoaError.stacktrace = Stacktrace;
	}

private:
	BugsnagError* CocoaError;
};
