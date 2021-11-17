// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "Modules/ModuleInterface.h"

class BUGSNAG_API FBugsnagModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
