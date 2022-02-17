#include "Scenario.h"

class ResumeSessionScenario : public Scenario
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
		UBugsnagFunctionLibrary::ResumeSession();
		volatile int* Pointer = nullptr;
		*Pointer = 42;
	}
};

REGISTER_SCENARIO(ResumeSessionScenario);
