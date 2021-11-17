#include "Scenario.h"

class RestrictSomeCrumbTypesScenario : public Scenario
{
public:
	void Configure() override
	{
		// none of these should happen
		Configuration->SetEnabledBreadcrumbTypes(EBugsnagEnabledBreadcrumbTypes::Navigation | EBugsnagEnabledBreadcrumbTypes::Request);
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("Message in a bottle"));
		UBugsnagFunctionLibrary::Notify("Dispatching a crumb", "none");
	}
};

REGISTER_SCENARIO(RestrictSomeCrumbTypesScenario);
