#include "Scenario.h"

#include "Dom/JsonObject.h"

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
		UBugsnagFunctionLibrary::SetContext("overhead view");

		// This doesn't really belong in this scenario, just checking that UBugsnagFunctionLibrary::GetBreadcrumbs() works
		TArray<TSharedRef<const IBugsnagBreadcrumb>> Breadcrumbs = UBugsnagFunctionLibrary::GetBreadcrumbs();
		assert(Breadcrumbs.Num() > 0);
		assert(!Breadcrumbs[0].GetMessage().IsEmpty());

		UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("About to read from a bad memory address"));

		volatile int* Pointer = nullptr;
		*Pointer = 42;
	}
};

REGISTER_SCENARIO(BadMemoryAccessScenario);
