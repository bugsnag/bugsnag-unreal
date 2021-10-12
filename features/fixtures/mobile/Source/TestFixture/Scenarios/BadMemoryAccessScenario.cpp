#include "Scenario.h"

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

		Configuration->AddOnBreadcrumb([](TSharedRef<IBugsnagBreadcrumb> Breadcrumb)
			{
				return true;
			});

		Configuration->AddOnSendError([](TSharedRef<IBugsnagEvent> Event)
			{
				Event->AddMetadata(TEXT("custom"), TEXT("configOnSendError"), MakeShareable(new FJsonValueString(TEXT("hello"))));
				return true;
			});

		Configuration->AddOnSession([](TSharedRef<IBugsnagSession> Session)
			{
				return true;
			});
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::AddOnBreadcrumb([](TSharedRef<IBugsnagBreadcrumb> Breadcrumb)
			{
				return true;
			});

		UBugsnagFunctionLibrary::AddOnSendError([](TSharedRef<IBugsnagEvent> Event)
			{
				return true;
			});

		UBugsnagFunctionLibrary::AddOnSession([](TSharedRef<IBugsnagSession> Session)
			{
				return true;
			});

		UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("About to read from a bad memory address"));

		UBugsnagFunctionLibrary::AddMetadata(TEXT("custom"), TEXT("someValue"), MakeShareable(new FJsonValueString(TEXT("foobar"))));

		FPlatformProcess::Sleep(0.5f); // Leave time for async breadcrumb / metadata I/O

		UBugsnagFunctionLibrary::SetContext("overhead view");

		volatile int* Pointer = nullptr;
		*Pointer = 42;
	}
};

REGISTER_SCENARIO(BadMemoryAccessScenario);
