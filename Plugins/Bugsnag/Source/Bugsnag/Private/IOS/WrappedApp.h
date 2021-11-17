// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "AppleBugsnagUtils.h"
#include "BugsnagApp.h"

#import <Bugsnag/BugsnagApp.h>

class FWrappedApp : virtual public IBugsnagApp
{
public:
	static TSharedRef<FWrappedApp> From(BugsnagApp* CocoaApp)
	{
		return MakeShared<FWrappedApp>(CocoaApp);
	}

	FWrappedApp(BugsnagApp* CocoaApp)
		: CocoaApp(CocoaApp)
	{
	}

	// binaryArch?: string;

	const TOptional<FString> GetBinaryArch() const
	{
		return OptionalFromNSString(CocoaApp.binaryArch);
	}

	void SetBinaryArch(const TOptional<FString>& Value)
	{
		CocoaApp.binaryArch = NSStringFromOptional(Value);
	}

	// buildUuid?: string;

	const TOptional<FString> GetBuildUuid() const
	{
		// Not supported by bugsnag-cocoa
		return TOptional<FString>();
	}

	void SetBuildUuid(const TOptional<FString>&)
	{
		// Not supported by bugsnag-cocoa
	}

	// bundleVersion?: string;

	const TOptional<FString> GetBundleVersion() const
	{
		return OptionalFromNSString(CocoaApp.bundleVersion);
	}

	void SetBundleVersion(const TOptional<FString>& Value)
	{
		CocoaApp.bundleVersion = NSStringFromOptional(Value);
	}

	// dsymUuid?: string;

	const TOptional<FString> GetDsymUuid() const
	{
		return OptionalFromNSString(CocoaApp.dsymUuid);
	}

	void SetDsymUuid(const TOptional<FString>& Value)
	{
		CocoaApp.dsymUuid = NSStringFromOptional(Value);
	}

	// id?: string;

	const TOptional<FString> GetId() const
	{
		return OptionalFromNSString(CocoaApp.id);
	}

	void SetId(const TOptional<FString>& Value)
	{
		CocoaApp.id = NSStringFromOptional(Value);
	}

	// releaseStage: string = "production";

	const TOptional<FString> GetReleaseStage() const
	{
		return OptionalFromNSString(CocoaApp.releaseStage);
	}

	void SetReleaseStage(const TOptional<FString>& Value)
	{
		CocoaApp.releaseStage = NSStringFromOptional(Value);
	}

	// type?: string;

	const TOptional<FString> GetType() const
	{
		return OptionalFromNSString(CocoaApp.type);
	}

	void SetType(const TOptional<FString>& Value)
	{
		CocoaApp.type = NSStringFromOptional(Value);
	}

	// version?: string;

	const TOptional<FString> GetVersion() const
	{
		return OptionalFromNSString(CocoaApp.version);
	}

	void SetVersion(const TOptional<FString>& Value)
	{
		CocoaApp.version = NSStringFromOptional(Value);
	}

	// versionCode?: number;

	const TOptional<int64> GetVersionCode() const
	{
		// Not supported by bugsnag-cocoa
		return TOptional<int64>();
	}

	void SetVersionCode(const TOptional<int64>& Value)
	{
		// Not supported by bugsnag-cocoa
	}

private:
	BugsnagApp* CocoaApp;
};
