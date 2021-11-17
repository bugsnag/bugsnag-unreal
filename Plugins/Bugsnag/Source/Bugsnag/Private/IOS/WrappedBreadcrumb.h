// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "AppleBugsnagUtils.h"
#include "BugsnagBreadcrumb.h"

#import <Bugsnag/BugsnagBreadcrumb.h>

class FWrappedBreadcrumb : public IBugsnagBreadcrumb
{
public:
	static TSharedRef<FWrappedBreadcrumb> From(BugsnagBreadcrumb* CocoaBreadcrumb)
	{
		return MakeShared<FWrappedBreadcrumb>(CocoaBreadcrumb);
	}

	FWrappedBreadcrumb(BugsnagBreadcrumb* CocoaBreadcrumb)
		: CocoaBreadcrumb(CocoaBreadcrumb)
	{
	}

	const FDateTime GetTimestamp() const override
	{
		return FDateTimeFromNSDate(CocoaBreadcrumb.timestamp);
	}

	const FString GetMessage() const override
	{
		return FStringFromNSString(CocoaBreadcrumb.message);
	}

	void SetMessage(const FString& Message) override
	{
		CocoaBreadcrumb.message = NSStringFromFString(Message);
	}

	EBugsnagBreadcrumbType GetType() const override
	{
		switch (CocoaBreadcrumb.type)
		{
		case BSGBreadcrumbTypeManual:
			return EBugsnagBreadcrumbType::Manual;
		case BSGBreadcrumbTypeError:
			return EBugsnagBreadcrumbType::Error;
		case BSGBreadcrumbTypeLog:
			return EBugsnagBreadcrumbType::Log;
		case BSGBreadcrumbTypeNavigation:
			return EBugsnagBreadcrumbType::Navigation;
		case BSGBreadcrumbTypeProcess:
			return EBugsnagBreadcrumbType::Process;
		case BSGBreadcrumbTypeRequest:
			return EBugsnagBreadcrumbType::Request;
		case BSGBreadcrumbTypeState:
			return EBugsnagBreadcrumbType::State;
		case BSGBreadcrumbTypeUser:
			return EBugsnagBreadcrumbType::User;
		}
	}

	static BSGBreadcrumbType ConvertType(EBugsnagBreadcrumbType Type)
	{
		switch (Type)
		{
		case EBugsnagBreadcrumbType::Manual:
			return BSGBreadcrumbTypeManual;
		case EBugsnagBreadcrumbType::Error:
			return BSGBreadcrumbTypeError;
		case EBugsnagBreadcrumbType::Log:
			return BSGBreadcrumbTypeLog;
		case EBugsnagBreadcrumbType::Navigation:
			return BSGBreadcrumbTypeNavigation;
		case EBugsnagBreadcrumbType::Process:
			return BSGBreadcrumbTypeProcess;
		case EBugsnagBreadcrumbType::Request:
			return BSGBreadcrumbTypeRequest;
		case EBugsnagBreadcrumbType::State:
			return BSGBreadcrumbTypeState;
		case EBugsnagBreadcrumbType::User:
			return BSGBreadcrumbTypeUser;
		}
	}

	void SetType(EBugsnagBreadcrumbType Type) override
	{
		CocoaBreadcrumb.type = ConvertType(Type);
	}

	TSharedPtr<FJsonObject> GetMetadata() const override
	{
		return FJsonObjectFromNSDictionary(CocoaBreadcrumb.metadata);
	}

	void SetMetadata(const TSharedPtr<FJsonObject>& Metadata) override
	{
		CocoaBreadcrumb.metadata = NSDictionaryFromFJsonObject(Metadata);
	}

private:
	BugsnagBreadcrumb* CocoaBreadcrumb;
};
