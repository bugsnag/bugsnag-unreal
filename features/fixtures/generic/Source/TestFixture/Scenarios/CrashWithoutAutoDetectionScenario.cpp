#include "Scenario.h"

#include <cstdlib>

class CrashWithoutAutoDetectionScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->SetAutoDetectErrors(false);
	}

	void Run() override
	{
		abort();
	}
};

REGISTER_SCENARIO(CrashWithoutAutoDetectionScenario);
