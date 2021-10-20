#include "Scenario.h"
#include <cstdlib>

class CrashAfterLaunchedScenario : public Scenario
{
public:
	void Run() override
	{
		UBugsnagFunctionLibrary::MarkLaunchCompleted();
		abort();
	}
};

REGISTER_SCENARIO(CrashAfterLaunchedScenario);
