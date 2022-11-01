// Copyright 2021 Bugsnag. All Rights Reserved.

#include "ApplePlatformConfiguration.h"

#include "AppleBreadcrumb.h"
#include "AppleBugsnagUtils.h"
#include "AppleEvent.h"
#include "AppleSession.h"
#include "Version.h"

#import <BugsnagPrivate/BugsnagInternals.h>

static BSGTelemetryOptions GetTelemetryTypes(EBugsnagTelemetryTypes Value)
{
	return (EnumHasAllFlags(Value, EBugsnagTelemetryTypes::InternalErrors) ? BSGTelemetryInternalErrors : 0);
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

static BSGEnabledBreadcrumbType GetEnabledBreadcrumbTypes(EBugsnagEnabledBreadcrumbTypes Value)
{
	// Note: There is no BSGEnabledBreadcrumbTypeManual
	return (EnumHasAllFlags(Value, EBugsnagEnabledBreadcrumbTypes::Error) ? BSGEnabledBreadcrumbTypeError : 0) |
		   (EnumHasAllFlags(Value, EBugsnagEnabledBreadcrumbTypes::Log) ? BSGEnabledBreadcrumbTypeLog : 0) |
		   (EnumHasAllFlags(Value, EBugsnagEnabledBreadcrumbTypes::Navigation) ? BSGEnabledBreadcrumbTypeNavigation : 0) |
		   (EnumHasAllFlags(Value, EBugsnagEnabledBreadcrumbTypes::Process) ? BSGEnabledBreadcrumbTypeProcess : 0) |
		   (EnumHasAllFlags(Value, EBugsnagEnabledBreadcrumbTypes::Request) ? BSGEnabledBreadcrumbTypeRequest : 0) |
		   (EnumHasAllFlags(Value, EBugsnagEnabledBreadcrumbTypes::State) ? BSGEnabledBreadcrumbTypeState : 0) |
		   (EnumHasAllFlags(Value, EBugsnagEnabledBreadcrumbTypes::User) ? BSGEnabledBreadcrumbTypeUser : 0);
}

static BugsnagErrorTypes* GetEnabledErrorTypes(FBugsnagErrorTypes EnabledErrorTypes)
{
	BugsnagErrorTypes* Types = [[BugsnagErrorTypes alloc] init];
	Types.appHangs = EnabledErrorTypes.bAppHangs;
	Types.ooms = EnabledErrorTypes.bOOMs;
	Types.unhandledExceptions = EnabledErrorTypes.bCrashes;
	Types.signals = EnabledErrorTypes.bCrashes;
	Types.cppExceptions = EnabledErrorTypes.bCrashes;
	Types.machExceptions = EnabledErrorTypes.bCrashes;
	Types.thermalKills = EnabledErrorTypes.bThermalKills;
	Types.unhandledRejections = NO;
	return Types;
}

BugsnagConfiguration* FApplePlatformConfiguration::Configuration(const TSharedRef<FBugsnagConfiguration>& Configuration)
{
	BugsnagConfiguration* CocoaConfig = [[BugsnagConfiguration alloc] initWithApiKey:NSStringFromFString(Configuration->GetApiKey())];

	CocoaConfig.autoDetectErrors = Configuration->GetAutoDetectErrors();

	CocoaConfig.autoTrackSessions = Configuration->GetAutoTrackSessions();

	if (Configuration->GetContext().IsSet())
	{
		CocoaConfig.context = NSStringFromFString(Configuration->GetContext().GetValue());
	}

	if (Configuration->GetDiscardClasses().Num())
	{
		CocoaConfig.discardClasses = NSSetFromFStrings(Configuration->GetDiscardClasses());
	}

	CocoaConfig.enabledBreadcrumbTypes = GetEnabledBreadcrumbTypes(Configuration->GetEnabledBreadcrumbTypes());

	CocoaConfig.enabledErrorTypes = GetEnabledErrorTypes(Configuration->GetEnabledErrorTypes());

	if (Configuration->GetEnabledReleaseStages().Num())
	{
		CocoaConfig.enabledReleaseStages = NSSetFromFStrings(Configuration->GetEnabledReleaseStages());
	}

	if (Configuration->GetRedactedKeys().Num())
	{
		CocoaConfig.redactedKeys = NSSetFromFStrings(Configuration->GetRedactedKeys());
	}

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

	CocoaConfig.telemetry = GetTelemetryTypes(Configuration->GetTelemetry());

	if (Configuration->GetReleaseStage().IsSet())
	{
		CocoaConfig.releaseStage = NSStringFromFString(Configuration->GetReleaseStage().GetValue());
	}

	if (Configuration->GetAppType().IsSet())
	{
		CocoaConfig.appType = NSStringFromFString(Configuration->GetAppType().GetValue());
	}

	if (Configuration->GetAppVersion().IsSet())
	{
		CocoaConfig.appVersion = NSStringFromFString(Configuration->GetAppVersion().GetValue());
	}

	if (Configuration->GetBundleVersion().IsSet())
	{
		CocoaConfig.bundleVersion = NSStringFromFString(Configuration->GetBundleVersion().GetValue());
	}

	CocoaConfig.endpoints = [[BugsnagEndpointConfiguration alloc]
		initWithNotify:NSStringFromFString(Configuration->GetEndpoints().GetNotify())
			  sessions:NSStringFromFString(Configuration->GetEndpoints().GetSessions())];

	if (!Configuration->GetUser().IsEmpty())
	{
		[CocoaConfig setUser:NSStringFromOptional(Configuration->GetUser().GetId())
				   withEmail:NSStringFromOptional(Configuration->GetUser().GetEmail())
					 andName:NSStringFromOptional(Configuration->GetUser().GetName())];
	}

	if (Configuration->FeatureFlags.Num() > 0)
	{
		NSMutableArray* CocoaFeatureFlags = [NSMutableArray array];
		for (const TPair<FString, TOptional<FString>>& Elem : Configuration->FeatureFlags)
		{
			[CocoaFeatureFlags addObject:[BugsnagFeatureFlag flagWithName:NSStringFromFString(Elem.Key)
																  variant:NSStringFromOptional(Elem.Value)]];
		}
		[CocoaConfig addFeatureFlags:CocoaFeatureFlags];
	}

	for (auto& Item : Configuration->GetMetadataValues())
	{
		NSError* Error = nil;
		id Object = NSDictionaryFromFJsonObject(Item.Value, &Error);
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

	for (auto Callback : Configuration->GetOnBreadcrumbCallbacks())
	{
		[CocoaConfig addOnBreadcrumbBlock:^BOOL(BugsnagBreadcrumb* _Nonnull Breadcrumb) {
			return Callback(FAppleBreadcrumb::From(Breadcrumb));
		}];
	}

	for (auto Callback : Configuration->GetOnSendErrorCallbacks())
	{
		[CocoaConfig addOnSendErrorBlock:^BOOL(BugsnagEvent* _Nonnull Event) {
			return Callback(FAppleEvent::From(Event));
		}];
	}

	for (auto Callback : Configuration->GetOnSessionCallbacks())
	{
		[CocoaConfig addOnSessionBlock:^BOOL(BugsnagSession* _Nonnull Session) {
			return Callback(FAppleSession::From(Session));
		}];
	}

	CocoaConfig.notifier =
		[[BugsnagNotifier alloc] initWithName:@BUGSNAG_UNREAL_NAME
									  version:@BUGSNAG_UNREAL_VERSION_STRING
										  url:@BUGSNAG_UNREAL_VERSION_STRING
								 dependencies:@[[[BugsnagNotifier alloc] init]]];

	return CocoaConfig;
}
