#include "Scenario.h"

class CancelNotifyFromCallback : public Scenario
{

public:
	void Run() override
	{
		UBugsnagFunctionLibrary::Notify("Missing builder", "Task requires at least two", [](TSharedRef<IBugsnagEvent> Event)
			{
				return false;
			});
	}
};

REGISTER_SCENARIO(CancelNotifyFromCallback)
