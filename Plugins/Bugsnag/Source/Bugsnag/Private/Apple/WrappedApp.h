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

	const TSharedPtr<FString> GetBinaryArch() const
	{
		return FStringPtrFromNSString(CocoaApp.binaryArch);
	}

	void SetBinaryArch(const TSharedPtr<FString>& Value)
	{
		CocoaApp.binaryArch = NSStringFromFStringPtr(Value);
	}

	// buildUuid?: string;

	const TSharedPtr<FString> GetBuildUuid() const
	{
		// Not supported by bugsnag-cocoa
		return nullptr;
	}

	void SetBuildUuid(const TSharedPtr<FString>&)
	{
		// Not supported by bugsnag-cocoa
	}

	// bundleVersion?: string;

	const TSharedPtr<FString> GetBundleVersion() const
	{
		return FStringPtrFromNSString(CocoaApp.bundleVersion);
	}

	void SetBundleVersion(const TSharedPtr<FString>& Value)
	{
		CocoaApp.bundleVersion = NSStringFromFStringPtr(Value);
	}

	// dsymUuid?: string;

	const TSharedPtr<FString> GetDsymUuid() const
	{
		return FStringPtrFromNSString(CocoaApp.dsymUuid);
	}

	void SetDsymUuid(const TSharedPtr<FString>& Value)
	{
		CocoaApp.dsymUuid = NSStringFromFStringPtr(Value);
	}

	// id?: string;

	const TSharedPtr<FString> GetId() const
	{
		return FStringPtrFromNSString(CocoaApp.id);
	}

	void SetId(const TSharedPtr<FString>& Value)
	{
		CocoaApp.id = NSStringFromFStringPtr(Value);
	}

	// releaseStage: string = "production";

	const TSharedPtr<FString> GetReleaseStage() const
	{
		return FStringPtrFromNSString(CocoaApp.releaseStage);
	}

	void SetReleaseStage(const TSharedPtr<FString>& Value)
	{
		CocoaApp.releaseStage = NSStringFromFStringPtr(Value);
	}

	// type?: string;

	const TSharedPtr<FString> GetType() const
	{
		return FStringPtrFromNSString(CocoaApp.type);
	}

	void SetType(const TSharedPtr<FString>& Value)
	{
		CocoaApp.type = NSStringFromFStringPtr(Value);
	}

	// version?: string;

	const TSharedPtr<FString> GetVersion() const
	{
		return FStringPtrFromNSString(CocoaApp.version);
	}

	void SetVersion(const TSharedPtr<FString>& Value)
	{
		CocoaApp.version = NSStringFromFStringPtr(Value);
	}

	// versionCode?: number;

	const TSharedPtr<int64> GetVersionCode() const
	{
		// Not supported by bugsnag-cocoa
		return nullptr;
	}

	void SetVersionCode(const TSharedPtr<int64>& Value)
	{
		// Not supported by bugsnag-cocoa
	}

private:
	BugsnagApp* CocoaApp;
};
