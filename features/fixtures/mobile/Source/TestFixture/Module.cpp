#include "CoreGlobals.h"
#include "Modules/ModuleManager.h"

class FTestFixture : public IModuleInterface
{
public:
	virtual void StartupModule() override{};

	virtual void ShutdownModule() override{};
};

IMPLEMENT_PRIMARY_GAME_MODULE(FTestFixture, TestFixture, "TestFixture");
