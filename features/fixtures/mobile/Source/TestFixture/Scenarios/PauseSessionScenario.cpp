#include "Scenario.h"

class PauseSessionScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->SetAutoTrackSessions(false);
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::StartSession();
		UBugsnagFunctionLibrary::PauseSession();
		volatile int* Pointer = nullptr;
		*Pointer = 42;
	}
};

REGISTER_SCENARIO(PauseSessionScenario);
