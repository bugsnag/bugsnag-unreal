// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

/**
 * The BugsnagEditor module exposes Bugsnag settings in Unreal Editor's Project Settings UI.
 */
class FBugsnagEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
