#include "Scenario.h"

class ReleaseStageEnabledScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->SetEnabledReleaseStages({TEXT("enabled")});
		Configuration->SetReleaseStage(FString(TEXT("enabled")));
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::Notify(TEXT("Error"), TEXT("This error should be reported"));
	}
};

REGISTER_SCENARIO(ReleaseStageEnabledScenario)
