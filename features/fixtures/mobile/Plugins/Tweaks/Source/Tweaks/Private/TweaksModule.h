#pragma once

#include "Modules/ModuleInterface.h"

class TWEAKS_API FTweaksModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
