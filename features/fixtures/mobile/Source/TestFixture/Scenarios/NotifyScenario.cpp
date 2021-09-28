#include "Scenario.h"

class NotifyScenario : public Scenario
{
public:
	void Run() override
	{
		UBugsnagFunctionLibrary::SetContext("pause menu");

		UBugsnagFunctionLibrary::Notify(TEXT("Internal Error"), TEXT("Does not compute"));
	}
};

REGISTER_SCENARIO(NotifyScenario);
