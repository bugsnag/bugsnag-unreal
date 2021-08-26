#include "BugsnagModule.h"
#include "BugsnagSettings.h"
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
	const UBugsnagSettings* Settings = GetDefault<UBugsnagSettings>();
	if (Settings->ApiKey.Len())
	{
		[Bugsnag startWithApiKey:@(TCHAR_TO_UTF8(*Settings->ApiKey))];
	}
#endif
}

void FBugsnagModule::ShutdownModule() {}

IMPLEMENT_MODULE(FBugsnagModule, Bugsnag)
