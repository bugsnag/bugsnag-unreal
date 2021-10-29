#include "Scenario.h"

class MaxConfigCrashScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->SetApiKey(TEXT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
		Configuration->SetVersionCode(22);
		Configuration->SetAppType(FString("mailer"));
		Configuration->SetAppVersion(FString("2.61.0"));
		Configuration->SetBundleVersion(FString("2.61.0.1"));
		Configuration->SetContext(FString("Main Menu"));
		Configuration->SetDiscardClasses({TEXT("ClassNotFoundException")});
		Configuration->SetEnabledReleaseStages({TEXT("beta04"), TEXT("prod")});
		Configuration->SetRedactedKeys({TEXT("pw")});
		Configuration->SetReleaseStage(FString("beta04"));
		Configuration->SetUser(FString("1255"), FString("j@example.com"), FString("jimmy"));
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
