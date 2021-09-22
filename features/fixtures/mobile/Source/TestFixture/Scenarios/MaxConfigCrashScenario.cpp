#include "Scenario.h"

class MaxConfigCrashScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->SetApiKey(TEXT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
		Configuration->SetVersionCode(22);
		Configuration->SetAppType(TEXT("mailer"));
		Configuration->SetAppVersion(TEXT("2.61.0"));
		Configuration->SetBundleVersion(TEXT("2.61.0.1"));
		Configuration->SetContext(TEXT("Main Menu"));
		Configuration->SetDiscardClasses({TEXT("ClassNotFoundException")});
		Configuration->SetEnabledReleaseStages({TEXT("beta04"), TEXT("prod")});
		Configuration->SetRedactedKeys({TEXT("pw")});
		Configuration->SetReleaseStage(TEXT("beta04"));
		Configuration->SetUser(TEXT("1255"), TEXT("j@example.com"), TEXT("jimmy"));
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("About to read from a bad memory address"));
		volatile int* Pointer = nullptr;
		*Pointer = 42;
		UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("This will never happen"));
	}
};

REGISTER_SCENARIO(MaxConfigCrashScenario);
