#include "Scenario.h"

#include "Dom/JsonObject.h"
#include "HAL/PlatformProcess.h"

class BadMemoryAccessScenario : public Scenario
{
public:
	void Configure() override
	{
		TSharedPtr<FJsonObject> Section1 = MakeShared<FJsonObject>();
		TSharedPtr<FJsonObject> Section2 = MakeShared<FJsonObject>();
		auto Value = new FJsonValueString("35");

		Section1->SetBoolField("cronut", false);
		Section1->SetNumberField("macaron", 3);
		Section2->SetStringField("forty", TEXT("40"));

		Configuration->AddMetadata("pastries", Section1);
		Configuration->AddMetadata("counters", Section2);
		Configuration->AddMetadata("counters", "thirty-five", MakeShareable(Value));
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("About to read from a bad memory address"));
		FPlatformProcess::Sleep(0.5f); // Leave time for async breadcrumb I/O

		UBugsnagFunctionLibrary::SetContext("overhead view");

		volatile int* Pointer = nullptr;
		*Pointer = 42;
	}
};

REGISTER_SCENARIO(BadMemoryAccessScenario);
