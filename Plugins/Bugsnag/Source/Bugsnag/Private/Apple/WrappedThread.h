#pragma once

#include "CoreMinimal.h"

#include "AppleBugsnagUtils.h"
#include "BugsnagThread.h"
#include "LogBugsnag.h"
#include "WrappedStackframe.h"

#import <Bugsnag/BugsnagThread.h>

class BUGSNAG_API FWrappedThread : public IBugsnagThread
{
public:
	static TSharedRef<FWrappedThread> From(BugsnagThread* CocoaThread)
	{
		return MakeShared<FWrappedThread>(CocoaThread);
	}

	FWrappedThread(BugsnagThread* CocoaThread)
		: CocoaThread(CocoaThread)
	{
	}

	// id: string;

	FString GetId() const override
	{
		return FStringFromNSString(CocoaThread.id);
	}

	void SetId(const FString& Value) override
	{
		CocoaThread.id = NSStringFromFString(Value);
	}

	// name: string;

	FString GetName() const override
	{
		return FStringFromNSString(CocoaThread.name);
	}

	void SetName(const FString& Value) override
	{
		CocoaThread.name = NSStringFromFString(Value);
	}

	// readonly errorReportingThread: boolean;

	bool GetErrorReportingThread() const override
	{
		return CocoaThread.errorReportingThread;
	}

	// type: ErrorType;

	EBugsnagErrorType GetErrorType() const override
	{
		switch (CocoaThread.type)
		{
		case BSGThreadTypeCocoa:
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
		for (BugsnagStackframe* Stackframe in CocoaThread.stacktrace)
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
		CocoaThread.stacktrace = Stacktrace;
	}

private:
	BugsnagThread* CocoaThread;
};
