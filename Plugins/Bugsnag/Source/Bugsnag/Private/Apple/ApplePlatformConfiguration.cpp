#include "ApplePlatformConfiguration.h"

#include "Version.h"

#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/LargeMemoryWriter.h"

#import <Bugsnag/BugsnagConfiguration.h>
#import <Bugsnag/BugsnagEndpointConfiguration.h>
#import <Bugsnag/BugsnagErrorTypes.h>

#import <BugsnagPrivate/BSGJSONSerialization.h>
#import <BugsnagPrivate/BugsnagConfiguration+Private.h>
#import <BugsnagPrivate/BugsnagNotifier.h>

static NSString* _Nullable NSStringFromFString(const FString& String, bool ReturnNilForEmpty = true)
{
	if (ReturnNilForEmpty && String.IsEmpty())
	{
		return nil;
	}
	return @(TCHAR_TO_UTF8(*String));
}

static NSSet* _Nullable NSSetFromFStrings(const TArray<FString>& Array, bool ReturnNilForEmpty = true)
{
	if (ReturnNilForEmpty && Array.Num() < 1)
	{
		return nil;
	}
	NSMutableSet* Set = [NSMutableSet set];
	for (const FString& Value : Array)
	{
		NSString* Object = NSStringFromFString(Value);
		if (Object)
		{
			[Set addObject:Object];
		}
	}
	return Set;
}

static id _Nullable NSObjectFromFJsonObject(const TSharedPtr<FJsonObject> JsonObject, NSError** Error)
{
	FLargeMemoryWriter Archive;
	TSharedRef<TJsonWriter<char>> JsonWriter = TJsonWriterFactory<char>::Create(&Archive);
	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter))
	{
		JsonWriter->Close();
		NSData* Data = [NSData dataWithBytesNoCopy:Archive.GetData() length:Archive.TotalSize() freeWhenDone:NO];
		return [BSGJSONSerialization JSONObjectWithData:Data options:0 error:Error];
	}
	return nil;
}

static BSGThreadSendPolicy GetThreadSendPolicy(EBugsnagSendThreadsPolicy Policy)
{
	switch (Policy)
	{
	case EBugsnagSendThreadsPolicy::All:
		return BSGThreadSendPolicyAlways;
	case EBugsnagSendThreadsPolicy::UnhandledOnly:
		return BSGThreadSendPolicyUnhandledOnly;
	case EBugsnagSendThreadsPolicy::Never:
		return BSGThreadSendPolicyNever;
	}
	return BSGThreadSendPolicyAlways;
}

static BSGEnabledBreadcrumbType GetEnabledBreadcrumbTypes(FBugsnagEnabledBreadcrumbTypes EnabledBreadcrumbTypes)
{
	return (EnabledBreadcrumbTypes.bError ? BSGEnabledBreadcrumbTypeError : 0) |
		   (EnabledBreadcrumbTypes.bLog ? BSGEnabledBreadcrumbTypeLog : 0) |
		   (EnabledBreadcrumbTypes.bNavigation ? BSGEnabledBreadcrumbTypeNavigation : 0) |
		   (EnabledBreadcrumbTypes.bProcess ? BSGEnabledBreadcrumbTypeProcess : 0) |
		   (EnabledBreadcrumbTypes.bRequest ? BSGEnabledBreadcrumbTypeRequest : 0) |
		   (EnabledBreadcrumbTypes.bState ? BSGEnabledBreadcrumbTypeState : 0) |
		   (EnabledBreadcrumbTypes.bUser ? BSGEnabledBreadcrumbTypeUser : 0);
}

static BugsnagErrorTypes* GetEnabledErrorTypes(FBugsnagErrorTypes EnabledErrorTypes)
{
	BugsnagErrorTypes* Types = [[BugsnagErrorTypes alloc] init];
	Types.appHangs = EnabledErrorTypes.bAppHangs;
	Types.ooms = EnabledErrorTypes.bOOMs;
	Types.unhandledExceptions = EnabledErrorTypes.bUnhandledExceptions;
	Types.signals = EnabledErrorTypes.bNativeCrashes;
	Types.cppExceptions = EnabledErrorTypes.bNativeCrashes;
	Types.machExceptions = EnabledErrorTypes.bNativeCrashes;
	Types.thermalKills = EnabledErrorTypes.bThermalKills;
	Types.unhandledRejections = NO;
	return Types;
}

BugsnagConfiguration* FApplePlatformConfiguration::Configuration(const TSharedPtr<FBugsnagConfiguration>& Configuration)
{
	BugsnagConfiguration* CocoaConfig = [[BugsnagConfiguration alloc] initWithApiKey:NSStringFromFString(Configuration->GetApiKey())];

	CocoaConfig.autoDetectErrors = Configuration->GetAutoDetectErrors();

	CocoaConfig.autoTrackSessions = Configuration->GetAutoTrackSessions();

	CocoaConfig.context = NSStringFromFString(Configuration->GetContext());

	CocoaConfig.discardClasses = NSSetFromFStrings(Configuration->GetDiscardClasses());

	CocoaConfig.enabledBreadcrumbTypes = GetEnabledBreadcrumbTypes(Configuration->GetEnabledBreadcrumbTypes());

	CocoaConfig.enabledErrorTypes = GetEnabledErrorTypes(Configuration->GetEnabledErrorTypes());

	CocoaConfig.enabledReleaseStages = NSSetFromFStrings(Configuration->GetEnabledReleaseStages());

	CocoaConfig.redactedKeys = NSSetFromFStrings(Configuration->GetRedactedKeys());

	if (Configuration->GetAppHangThresholdMillis() == FBugsnagConfiguration::AppHangThresholdFatalOnly)
	{
		CocoaConfig.appHangThresholdMillis = BugsnagAppHangThresholdFatalOnly;
	}
	else
	{
		CocoaConfig.appHangThresholdMillis = Configuration->GetAppHangThresholdMillis();
	}

	CocoaConfig.sendThreads = BSGThreadSendPolicy(Configuration->GetSendThreads());

	CocoaConfig.launchDurationMillis = Configuration->GetLaunchDurationMillis();

	CocoaConfig.sendLaunchCrashesSynchronously = Configuration->GetSendLaunchCrashesSynchronously();

	CocoaConfig.maxBreadcrumbs = Configuration->GetMaxBreadcrumbs();

	CocoaConfig.maxPersistedEvents = Configuration->GetMaxPersistedEvents();

	CocoaConfig.maxPersistedSessions = Configuration->GetMaxPersistedSessions();

	CocoaConfig.persistUser = Configuration->GetPersistUser();

	CocoaConfig.releaseStage = NSStringFromFString(Configuration->GetReleaseStage());

	CocoaConfig.appType = NSStringFromFString(Configuration->GetAppType());

	CocoaConfig.appVersion = NSStringFromFString(Configuration->GetAppVersion());

	CocoaConfig.bundleVersion = NSStringFromFString(Configuration->GetBundleVersion());

	CocoaConfig.endpoints = [[BugsnagEndpointConfiguration alloc]
		initWithNotify:NSStringFromFString(Configuration->GetEndpoints().GetNotify())
			  sessions:NSStringFromFString(Configuration->GetEndpoints().GetSessions())];

	if (!Configuration->GetUser().IsEmpty())
	{
		[CocoaConfig setUser:NSStringFromFString(Configuration->GetUser().GetId())
				   withEmail:NSStringFromFString(Configuration->GetUser().GetEmail())
					 andName:NSStringFromFString(Configuration->GetUser().GetName())];
	}

	for (auto& Item : Configuration->GetMetadataValues())
	{
		NSError* Error = nil;
		id Object = NSObjectFromFJsonObject(Item.Value, &Error);
		if (Object)
		{
			[CocoaConfig addMetadata:Object toSection:NSStringFromFString(Item.Key)];
		}
		else
		{
			UE_LOG(LogBugsnag, Warning, TEXT("Invalid configuration. Metadata section \"%s\" could not be parsed..."), *Item.Key);
			UE_LOG(LogBugsnag, Warning, TEXT("Parsing error: %s"), UTF8_TO_TCHAR(Error.description.UTF8String));
		}
	}

	for (auto& Callback : Configuration->GetOnBreadcrumbCallbacks())
	{
		[CocoaConfig addOnBreadcrumbBlock:^BOOL(BugsnagBreadcrumb* _Nonnull Breadcrumb) {
			// TODO: Convert BugsnagBreadcrumb to IBugsnagBreadcrumb
			return Callback((class IBugsnagBreadcrumb*)nullptr);
		}];
	}

	for (auto& Callback : Configuration->GetOnSendErrorCallbacks())
	{
		[CocoaConfig addOnSendErrorBlock:^BOOL(BugsnagEvent* _Nonnull Event) {
			// TODO: Convert BugsnagEvent to IBugsnagEvent
			return Callback((class IBugsnagEvent*)nullptr);
		}];
	}

	for (auto& Callback : Configuration->GetOnSessionCallbacks())
	{
		[CocoaConfig addOnSessionBlock:^BOOL(BugsnagSession* _Nonnull Session) {
			// TODO: Convert BugsnagSession to IBugsnagSession
			return Callback((class IBugsnagSession*)nullptr);
		}];
	}

	CocoaConfig.notifier =
		[[BugsnagNotifier alloc] initWithName:@BUGSNAG_UNREAL_NAME
									  version:@BUGSNAG_UNREAL_VERSION_STRING
										  url:@BUGSNAG_UNREAL_VERSION_STRING
								 dependencies:@[[[BugsnagNotifier alloc] init]]];

	return CocoaConfig;
}
