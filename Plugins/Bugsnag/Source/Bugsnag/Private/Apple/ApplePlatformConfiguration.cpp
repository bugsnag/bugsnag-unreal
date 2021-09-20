#include "ApplePlatformConfiguration.h"

#include "AppleBugsnagUtils.h"
#include "Version.h"
#include "WrappedBreadcrumb.h"
#include "WrappedSession.h"

#import <Bugsnag/BugsnagConfiguration.h>
#import <Bugsnag/BugsnagEndpointConfiguration.h>
#import <Bugsnag/BugsnagErrorTypes.h>

#import <BugsnagPrivate/BSGJSONSerialization.h>
#import <BugsnagPrivate/BugsnagConfiguration+Private.h>
#import <BugsnagPrivate/BugsnagNotifier.h>

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

	if (!Configuration->GetContext().IsEmpty())
	{
		CocoaConfig.context = NSStringFromFString(Configuration->GetContext());
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

	if (!Configuration->GetReleaseStage().IsEmpty())
	{
		CocoaConfig.releaseStage = NSStringFromFString(Configuration->GetReleaseStage());
	}

	if (!Configuration->GetAppType().IsEmpty())
	{
		CocoaConfig.appType = NSStringFromFString(Configuration->GetAppType());
	}

	if (!Configuration->GetAppVersion().IsEmpty())
	{
		CocoaConfig.appVersion = NSStringFromFString(Configuration->GetAppVersion());
	}

	if (!Configuration->GetBundleVersion().IsEmpty())
	{
		CocoaConfig.bundleVersion = NSStringFromFString(Configuration->GetBundleVersion());
	}

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

	for (auto& Callback : Configuration->GetOnBreadcrumbCallbacks())
	{
		[CocoaConfig addOnBreadcrumbBlock:^BOOL(BugsnagBreadcrumb* _Nonnull Breadcrumb) {
			return Callback(FWrappedBreadcrumb::From(Breadcrumb).Get());
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
			return Callback(FWrappedSession::From(Session).Get());
		}];
	}

	CocoaConfig.notifier =
		[[BugsnagNotifier alloc] initWithName:@BUGSNAG_UNREAL_NAME
									  version:@BUGSNAG_UNREAL_VERSION_STRING
										  url:@BUGSNAG_UNREAL_VERSION_STRING
								 dependencies:@[[[BugsnagNotifier alloc] init]]];

	return CocoaConfig;
}
