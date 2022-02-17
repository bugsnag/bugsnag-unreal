#include "Scenario.h"

class CancelSessionScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->SetAutoTrackSessions(false);
		Configuration->AddOnSession([](TSharedRef<IBugsnagSession> Session)
			{
				return false;
			});
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::StartSession();
		UBugsnagFunctionLibrary::Notify("Incorrect", "Missing token");
	}
};

REGISTER_SCENARIO(CancelSessionScenario);
