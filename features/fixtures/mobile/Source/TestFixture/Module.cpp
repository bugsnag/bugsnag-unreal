#include "CoreGlobals.h"
#include "Modules/ModuleManager.h"

#if PLATFORM_IOS

#include <objc/runtime.h>

static void (*UIAlertController_addAction_orig)(id self, SEL _cmd, UIAlertAction* action);

static void UIAlertController_addAction_swizzled(id self, SEL _cmd, UIAlertAction* action)
{
	if ([action.title isEqual:@"Cancel"])
	{
		// Awful hack to stop Appium's send_keys action dismissing the text entry alert via the cancel button.
		return;
	}
	UIAlertController_addAction_orig(self, _cmd, action);
}

#endif

class FTestFixture : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
#if PLATFORM_IOS
		UIAlertController_addAction_orig = (void (*)(id, SEL, UIAlertAction*))method_setImplementation(
			class_getInstanceMethod([UIAlertController class], NSSelectorFromString(@"addAction:")),
			(IMP)UIAlertController_addAction_swizzled);
#endif
	};

	virtual void ShutdownModule() override{};
};

IMPLEMENT_PRIMARY_GAME_MODULE(FTestFixture, TestFixture, "TestFixture");
