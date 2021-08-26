#include "BugsnagEditorModule.h"
#include "BugsnagSettings.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "Bugsnag"

void FBugsnagEditorModule::StartupModule()
{
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	if (SettingsModule != nullptr)	
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "Bugsnag",
			LOCTEXT("BugsnagSettingsName", "Bugsnag"),
			LOCTEXT("BugsnagSettingsDescription", "Configure the Bugsnag plug-in"),
			GetMutableDefault<UBugsnagSettings>()
		);
	}
}

void FBugsnagEditorModule::ShutdownModule()
{
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Bugsnag");
	}
}


IMPLEMENT_MODULE(FBugsnagEditorModule, BugsnagEditor);

#undef LOCTEXT_NAMESPACE
