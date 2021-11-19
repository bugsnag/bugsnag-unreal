#include "Scenario.h"

class DiscardClassesHandledErrorsScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->SetDiscardClasses({TEXT("AnnoyingError")});
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::Notify(TEXT("AnnoyingError"), TEXT("This error is annoying and should be discarded"));

		UBugsnagFunctionLibrary::Notify(TEXT("ImportantError"), TEXT("This error is too important be discarded"));
	}
};

REGISTER_SCENARIO(DiscardClassesHandledErrorsScenario)
