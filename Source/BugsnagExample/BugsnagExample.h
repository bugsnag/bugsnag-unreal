#pragma once

#include "CoreMinimal.h"

class FBugsnagExampleModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
