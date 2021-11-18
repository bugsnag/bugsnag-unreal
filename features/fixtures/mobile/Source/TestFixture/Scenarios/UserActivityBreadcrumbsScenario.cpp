#include "Scenario.h"

#include "UserActivityTracking.h"

class UserActivityBreadcrumbsScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->SetEnabledBreadcrumbTypes(EBugsnagEnabledBreadcrumbTypes::User);
	}

	void Run() override
	{
		FUserActivityTracking::SetActivity(FUserActivity("Cheat mode enabled"));

		UBugsnagFunctionLibrary::Notify(TEXT("Notify"), TEXT(""));
	}
};

REGISTER_SCENARIO(UserActivityBreadcrumbsScenario);
