#include "Scenario.h"

class BadMemoryAccessScenario : public Scenario
{
public:
	void Run() override
	{
		volatile int* Pointer = nullptr;
		*Pointer = 42;
	}
};

REGISTER_SCENARIO(BadMemoryAccessScenario);
