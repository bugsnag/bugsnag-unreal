#include "ApplePlatformBugsnag.h"

#include "AppleBugsnagUtils.h"
#include "ApplePlatformConfiguration.h"
#include "WrappedBreadcrumb.h"
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
	[Bugsnag startWithConfiguration:FApplePlatformConfiguration::Configuration(Configuration)];
}

void FApplePlatformBugsnag::Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace,
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

	[Client notifyInternal:Event block:^BOOL(BugsnagEvent* _Nonnull CocoaEvent) {
		return true; // TODO: if (Callback) return Callback(FWrappedEvent::From(CocoaEvent));
	}];
}

const FString FApplePlatformBugsnag::GetContext()
{
	return TEXT("");
}

void FApplePlatformBugsnag::SetContext(const FString& Context)
{
}

const TSharedPtr<FBugsnagUser> FApplePlatformBugsnag::GetUser()
{
	return nullptr;
}

void FApplePlatformBugsnag::SetUser(const FString& Id, const FString& Email, const FString& Name)
{
}

void FApplePlatformBugsnag::AddMetadata(const FString& Section, const TSharedPtr<FJsonObject>& Metadata)
{
}

void FApplePlatformBugsnag::AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value)
{
}

TSharedPtr<FJsonObject> FApplePlatformBugsnag::GetMetadata(const FString& Section)
{
	return nullptr;
}

TSharedPtr<FJsonValue> FApplePlatformBugsnag::GetMetadata(const FString& Section, const FString& Key)
{
	return nullptr;
}

void FApplePlatformBugsnag::ClearMetadata(const FString& Section)
{
}

void FApplePlatformBugsnag::ClearMetadata(const FString& Section, const FString& Key)
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
}

TSharedPtr<FBugsnagLastRunInfo> FApplePlatformBugsnag::GetLastRunInfo()
{
	return nullptr;
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

void FApplePlatformBugsnag::AddOnBreadcrumb(const FBugsnagOnBreadcrumbCallback& Callback)
{
}

void FApplePlatformBugsnag::AddOnSendError(const FBugsnagOnErrorCallback& Callback)
{
}

void FApplePlatformBugsnag::AddOnSession(const FBugsnagOnSessionCallback& Callback)
{
	[Bugsnag addOnSessionBlock:^BOOL(BugsnagSession* _Nonnull Session) {
		return Callback(FWrappedSession::From(Session));
	}];
}
