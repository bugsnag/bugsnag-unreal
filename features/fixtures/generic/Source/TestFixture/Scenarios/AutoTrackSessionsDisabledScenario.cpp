#include "Scenario.h"

class AutoTrackSessionsDisabledScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->SetAutoTrackSessions(false);
	}

	void Run() override
	{
		// intentionally empty
	}
};

REGISTER_SCENARIO(AutoTrackSessionsDisabledScenario);
