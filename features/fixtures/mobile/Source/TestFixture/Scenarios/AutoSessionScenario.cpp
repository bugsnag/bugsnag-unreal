#include "Scenario.h"

/**
 * Test that a session is automatically sent at launch by default
 */
class AutoSessionScenario : public Scenario
{
	void Run() override
	{
		// intentionally empty, only awaiting a session
	}
};

REGISTER_SCENARIO(AutoSessionScenario)
