#include "BugsnagModule.h"
#include "BugsnagFunctionLibrary.h"

#include "Modules/ModuleManager.h"

void FBugsnagModule::StartupModule()
{
	TSharedPtr<FBugsnagConfiguration> Configuration = FBugsnagConfiguration::Load();
	if (Configuration.IsValid() && !Configuration->GetApiKey().IsEmpty())
	{
		UBugsnagFunctionLibrary::Start(Configuration.ToSharedRef());
	}
}

void FBugsnagModule::ShutdownModule() {}

IMPLEMENT_MODULE(FBugsnagModule, Bugsnag)
