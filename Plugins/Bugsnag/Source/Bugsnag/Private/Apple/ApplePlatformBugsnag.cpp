#include "ApplePlatformBugsnag.h"

#include "AppleBugsnagUtils.h"
#include "ApplePlatformConfiguration.h"
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

void FApplePlatformBugsnag::Start(const TSharedPtr<FBugsnagConfiguration>& Configuration)
{
	FWrappedMetadataStore::CocoaStore = [Bugsnag startWithConfiguration:FApplePlatformConfiguration::Configuration(Configuration)];
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

const TSharedPtr<FString> FApplePlatformBugsnag::GetContext()
{
	return nullptr;
}

void FApplePlatformBugsnag::SetContext(const TSharedPtr<FString>& Context)
{
}

const FBugsnagUser FApplePlatformBugsnag::GetUser()
{
	return FBugsnagUser();
}

void FApplePlatformBugsnag::SetUser(const FString& Id, const FString& Email, const FString& Name)
{
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

void FApplePlatformBugsnag::AddOnSendError(FBugsnagOnErrorCallback Callback)
{
	[Bugsnag.configuration addOnSendErrorBlock:^BOOL(BugsnagEvent* _Nonnull Event) {
		return Callback(FWrappedEvent::From(Event));
	}];
}

void FApplePlatformBugsnag::AddOnSession(FBugsnagOnSessionCallback Callback)
{
	[Bugsnag addOnSessionBlock:^BOOL(BugsnagSession* _Nonnull Session) {
		return Callback(FWrappedSession::From(Session));
	}];
}
