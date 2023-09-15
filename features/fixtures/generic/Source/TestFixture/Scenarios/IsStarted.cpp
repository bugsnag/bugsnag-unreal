#include "Scenario.h"

class IsStarted : public Scenario
{
public:
	void Run() override
	{
        if(Configuration->GetIsStarted())
        {
            UBugsnagFunctionLibrary::Notify(TEXT("isStarted"), TEXT("isStarted"));
        }
        else 
        {
            UBugsnagFunctionLibrary::Notify(TEXT("isNotStarted"), TEXT("isNotStarted"));
        }
	}
};

REGISTER_SCENARIO(IsStarted);
