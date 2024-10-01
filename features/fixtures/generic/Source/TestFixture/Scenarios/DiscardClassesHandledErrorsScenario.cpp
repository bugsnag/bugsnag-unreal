#include "Scenario.h"
#include "BugsnagFunctionLibrary.h"
#include "Logging/LogMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogDiscardClasses, Log, All);

class DiscardClassesHandledErrorsScenario : public Scenario
{
public:
	void Configure() override
	{
		UE_LOG(LogDiscardClasses, Log, TEXT("RICHlog Configuring DiscardClasses with 'AnnoyingError'"));

		Configuration->SetDiscardClasses({TEXT("AnnoyingError")});

		UE_LOG(LogDiscardClasses, Log, TEXT("RICHlog DiscardClasses configuration completed"));
	}

	void Run() override
	{
		UE_LOG(LogDiscardClasses, Log, TEXT("RICHlog Running scenario: notifying 'AnnoyingError'"));
		UBugsnagFunctionLibrary::Notify(TEXT("AnnoyingError"), TEXT("This error is annoying and should be discarded"));

		UE_LOG(LogDiscardClasses, Log, TEXT("RICHlog Notifying 'ImportantError'"));
		UBugsnagFunctionLibrary::Notify(TEXT("ImportantError"), TEXT("This error is too important be discarded"));
		UE_LOG(LogDiscardClasses, Log, TEXT("RICHlog Notifying 'DONE ImportantError'"));

	}
};

REGISTER_SCENARIO(DiscardClassesHandledErrorsScenario)
