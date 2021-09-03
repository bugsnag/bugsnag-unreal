#include "BugsnagModule.h"
#include "BugsnagConfiguration.h"
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
	TSharedPtr<FBugsnagConfiguration> Configuration = FBugsnagConfiguration::Load();
	if (Configuration.IsValid() && !Configuration->GetApiKey().IsEmpty())
	{
		[Bugsnag startWithApiKey:@(TCHAR_TO_UTF8(*Configuration->GetApiKey()))];
	}
#endif
}

void FBugsnagModule::ShutdownModule() {}

IMPLEMENT_MODULE(FBugsnagModule, Bugsnag)
