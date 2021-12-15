#include "Scenario.h"

class RestrictAllCrumbTypesScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->SetEnabledBreadcrumbTypes(EBugsnagEnabledBreadcrumbTypes::None);
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("Message in a bottle"), EBugsnagBreadcrumbType::Navigation);
		UBugsnagFunctionLibrary::Notify("Dispatching a crumb", "none");
	}
};

REGISTER_SCENARIO(RestrictAllCrumbTypesScenario);
