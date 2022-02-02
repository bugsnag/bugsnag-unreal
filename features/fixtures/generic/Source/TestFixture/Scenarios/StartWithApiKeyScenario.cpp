#include "Scenario.h"

class StartWithApiKeyScenario : public Scenario
{
public:
	void Configure() override
	{
		// Modify UBugsnagSettings to simulate options being set in DefaultEngine.ini
		UBugsnagSettings* Settings = GetMutableDefault<UBugsnagSettings>();
		Settings->NotifyEndpoint = NotifyEndpoint;
		Settings->SessionsEndpoint = SessionsEndpoint;
		Settings->Context = TEXT("FromSettings");
	}

	void StartBugsnag() override
	{
		UBugsnagFunctionLibrary::Start(ApiKey);
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::Notify(TEXT("Game over"), TEXT("You died"));
	}
};

REGISTER_SCENARIO(StartWithApiKeyScenario);
