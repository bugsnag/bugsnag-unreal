// Copyright 2021 Bugsnag. All Rights Reserved.

#include "ApplePlatformBugsnag.h"

#include "AppleBugsnagUtils.h"
#include "ApplePlatformConfiguration.h"
#include "BugsnagConstants.h"
#include "BugsnagUtils.h"
#include "WrappedBreadcrumb.h"
#include "WrappedEvent.h"
#include "WrappedSession.h"

#include "HAL/PlatformStackWalk.h"

#import <Bugsnag/Bugsnag.h>
#import <BugsnagPrivate/BSG_KSSystemInfo.h>
#import <BugsnagPrivate/Bugsnag+Private.h>
#import <BugsnagPrivate/BugsnagBreadcrumbs.h>
#import <BugsnagPrivate/BugsnagClient+Private.h>
#import <BugsnagPrivate/BugsnagError+Private.h>
#import <BugsnagPrivate/BugsnagEvent+Private.h>
#import <BugsnagPrivate/BugsnagHandledState.h>
#import <BugsnagPrivate/BugsnagStackframe+Private.h>
#import <BugsnagPrivate/BugsnagThread+Private.h>

DEFINE_PLATFORM_BUGSNAG(FApplePlatformBugsnag);

#if UE_EDITOR

#import <Bugsnag/BSG_KSCrashReportWriter.h>
#import <BugsnagPrivate/BugsnagConfiguration+Private.h>

#define IS_PIE_WORLD_KEY "isPlayInEditorWorld"
#define UNREAL_ENGINE_KEY "unrealEngine"

static void FApplePlatformBugsnag_OnCrashHandler(const BSG_KSCrashReportWriter* writer)
{
	writer->beginObject(writer, UNREAL_ENGINE_KEY);
	writer->addBooleanElement(writer, IS_PIE_WORLD_KEY, GIsPlayInEditorWorld);
	writer->endContainer(writer);
}

#endif

void FApplePlatformBugsnag::Start(const TSharedRef<FBugsnagConfiguration>& Configuration)
{
	BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
#if UE_EDITOR
	if (!FApp::IsGame())
	{
		UE_LOG(LogBugsnag, Log, TEXT("App hang detection is disabled in the Editor"));
		CocoaConfig.enabledErrorTypes.appHangs = NO;

		UE_LOG(LogBugsnag, Log, TEXT("Crashes in the Editor outside of a PIE session will not be reported"));
		CocoaConfig.onCrashHandler = &FApplePlatformBugsnag_OnCrashHandler;
		BugsnagOnSendErrorBlock OnSend = ^BOOL(BugsnagEvent* _Nonnull event) {
			if ([[event getMetadataFromSection:@UNREAL_ENGINE_KEY withKey:@IS_PIE_WORLD_KEY] isEqual:@NO])
			{
				UE_LOG(LogBugsnag, Log, TEXT("Discarding crash in Editor outside of PIE session"));
				return NO;
			}
			return YES;
		};
		// Directly accessing `onSendBlocks` because we need this block to run first
		[CocoaConfig.onSendBlocks insertObject:OnSend atIndex:0];
	}
	else
	{
		// The Editor may have been launched in game mode, e.g. `UE4Editor <uproject> -game`
	}
#endif
	BugsnagClient* Client = [Bugsnag startWithConfiguration:CocoaConfig];
	[Client addRuntimeVersionInfo:NSStringFromFString(BugsnagUtils::GetUnrealEngineVersion())
						  withKey:NSStringFromFString(BugsnagConstants::UnrealEngine)];
	FWrappedMetadataStore::CocoaStore = Client;
}

void FApplePlatformBugsnag::Notify(
	const FString& ErrorClass,
	const FString& Message,
	const TArray<uint64>& StackTrace,
	const FBugsnagOnErrorCallback& Callback)
{
	BugsnagClient* Client = Bugsnag.client;

	NSDictionary* SystemInfo = [BSG_KSSystemInfo systemInfo];

	NSMutableArray<NSNumber*>* Addresses = [NSMutableArray arrayWithCapacity:StackTrace.Num()];
	for (uint64 Address : StackTrace)
	{
		[Addresses addObject:@(Address)];
	}

	BOOL RecordAllThreads = Client.configuration.sendThreads == BSGThreadSendPolicyAlways;
	NSArray* Threads = RecordAllThreads ? [BugsnagThread allThreads:YES callStackReturnAddresses:Addresses] : @[];

	BugsnagError* Error = [[BugsnagError alloc] initWithErrorClass:NSStringFromFString(ErrorClass)
													  errorMessage:NSStringFromFString(Message)
														 errorType:BSGErrorTypeCocoa
														stacktrace:[BugsnagStackframe stackframesWithCallStackReturnAddresses:Addresses]];

	BugsnagHandledState* HandledState = [BugsnagHandledState handledStateWithSeverityReason:HandledError
																				   severity:BSGSeverityWarning
																				  attrValue:nil];

	BugsnagEvent* Event = [[BugsnagEvent alloc] initWithApp:[Client generateAppWithState:SystemInfo]
													 device:[Client generateDeviceWithState:SystemInfo]
											   handledState:HandledState
													   user:Client.user
												   metadata:[Client.metadata deepCopy]
												breadcrumbs:Client.breadcrumbs.breadcrumbs ?: @[]
													 errors:@[Error]
													threads:Threads
													session:nil /* set by -[BugsnagClient notifyInternal:block:] */];
	Event.apiKey = Client.configuration.apiKey;
	Event.context = Client.context;

	BugsnagOnErrorBlock Block = nil;
	if (Callback)
	{
		Block = ^BOOL(BugsnagEvent* _Nonnull CocoaEvent) {
			return Callback(FWrappedEvent::From(CocoaEvent));
		};
	}

	[Client notifyInternal:Event
					 block:Block];
}

const TOptional<FString> FApplePlatformBugsnag::GetContext()
{
	return OptionalFromNSString(Bugsnag.context);
}

void FApplePlatformBugsnag::SetContext(const TOptional<FString>& Context)
{
	Bugsnag.context = NSStringFromOptional(Context);
}

const FBugsnagUser FApplePlatformBugsnag::GetUser()
{
	return FBugsnagUser(
		OptionalFromNSString(Bugsnag.user.id),
		OptionalFromNSString(Bugsnag.user.email),
		OptionalFromNSString(Bugsnag.user.name));
}

void FApplePlatformBugsnag::SetUser(const FString& Id, const FString& Email, const FString& Name)
{
	[Bugsnag setUser:Id.IsEmpty() ? nil : NSStringFromFString(Id)
		   withEmail:Email.IsEmpty() ? nil : NSStringFromFString(Email)
			 andName:Name.IsEmpty() ? nil : NSStringFromFString(Name)];
}

void FApplePlatformBugsnag::LeaveBreadcrumb(const FString& Message, const TSharedPtr<FJsonObject>& Metadata, EBugsnagBreadcrumbType Type)
{
	[Bugsnag leaveBreadcrumbWithMessage:NSStringFromFString(Message)
							   metadata:NSDictionaryFromFJsonObject(Metadata)
								andType:FWrappedBreadcrumb::ConvertType(Type)];
}

TArray<TSharedRef<const IBugsnagBreadcrumb>> FApplePlatformBugsnag::GetBreadcrumbs()
{
	TArray<TSharedRef<const IBugsnagBreadcrumb>> Result;
	for (BugsnagBreadcrumb* Breadcrumb in Bugsnag.breadcrumbs)
	{
		Result.Add(FWrappedBreadcrumb::From(Breadcrumb));
	}
	return Result;
}

void FApplePlatformBugsnag::MarkLaunchCompleted()
{
	[Bugsnag markLaunchCompleted];
}

TSharedPtr<FBugsnagLastRunInfo> FApplePlatformBugsnag::GetLastRunInfo()
{
	BugsnagLastRunInfo* LastRunInfo = Bugsnag.lastRunInfo;
	if (!LastRunInfo)
	{
		return nullptr;
	}
	return MakeShareable(new FBugsnagLastRunInfo(
		LastRunInfo.consecutiveLaunchCrashes,
		LastRunInfo.crashed,
		LastRunInfo.crashedDuringLaunch));
}

void FApplePlatformBugsnag::StartSession()
{
	[Bugsnag startSession];
}

void FApplePlatformBugsnag::PauseSession()
{
	[Bugsnag pauseSession];
}

bool FApplePlatformBugsnag::ResumeSession()
{
	return [Bugsnag resumeSession];
}

void FApplePlatformBugsnag::AddOnBreadcrumb(FBugsnagOnBreadcrumbCallback Callback)
{
	[Bugsnag addOnBreadcrumbBlock:^BOOL(BugsnagBreadcrumb* _Nonnull Breadcrumb) {
		return Callback(FWrappedBreadcrumb::From(Breadcrumb));
	}];
}

void FApplePlatformBugsnag::AddOnSession(FBugsnagOnSessionCallback Callback)
{
	[Bugsnag addOnSessionBlock:^BOOL(BugsnagSession* _Nonnull Session) {
		return Callback(FWrappedSession::From(Session));
	}];
}
