#include "Scenario.h"

#include <cstdlib>

class IgnoreErrorClassScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->SetApiKey(TEXT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
		Configuration->SetDiscardClasses({TEXT("SIGABRT")});
	}

	void Run() override
	{
		abort();
		UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("This will never happen"));
	}
};

REGISTER_SCENARIO(IgnoreErrorClassScenario);
