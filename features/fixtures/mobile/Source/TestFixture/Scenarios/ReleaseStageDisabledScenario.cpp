#include "Scenario.h"

class ReleaseStageDisabledScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->SetEnabledReleaseStages({TEXT("enabled")});
		Configuration->SetReleaseStage(FString(TEXT("disabled")));
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::Notify(TEXT("Error"), TEXT("This error should be discarded"));
	}
};

REGISTER_SCENARIO(ReleaseStageDisabledScenario)
