#include "BugsnagModule.h"
#include "Modules/ModuleManager.h"

// TODO: Remove this PLATFORM_APPLE section
#if PLATFORM_APPLE
#import <Bugsnag/Bugsnag.h>
#import <BugsnagPrivate/BSG_KSSystemInfo.h>
#endif

void FBugsnagModule::StartupModule()
{
// TODO: Remove this PLATFORM_APPLE section
#if PLATFORM_APPLE
	[Bugsnag startWithApiKey:@"NotAnApiKey"];
#endif
}

void FBugsnagModule::ShutdownModule() {}

IMPLEMENT_MODULE(FBugsnagModule, Bugsnag)
