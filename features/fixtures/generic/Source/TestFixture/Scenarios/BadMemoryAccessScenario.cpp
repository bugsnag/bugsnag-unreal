#include "Scenario.h"

#include "HAL/PlatformProcess.h"

class BadMemoryAccessScenario : public Scenario
{
public:
	void Configure() override
	{
		TSharedRef<FJsonObject> Section1 = MakeShared<FJsonObject>();
		TSharedRef<FJsonObject> Section2 = MakeShared<FJsonObject>();
		auto Value = new FJsonValueString("35");

		Section1->SetBoolField("cronut", false);
		Section1->SetNumberField("macaron", 3);
		Section2->SetStringField("forty", TEXT("40"));

		Configuration->AddFeatureFlag(TEXT("Testing"));
		Configuration->AddFeatureFlag(TEXT("fc1"), FString(TEXT("blue")));
		TArray<FBugsnagFeatureFlag> MoreFlags = {
			FBugsnagFeatureFlag(TEXT("fc1")),
			FBugsnagFeatureFlag(TEXT("fc2"), FString(TEXT("teal"))),
			FBugsnagFeatureFlag(TEXT("nope"))};
		Configuration->AddFeatureFlags(MoreFlags);

		Configuration->AddMetadata("pastries", Section1);
		Configuration->AddMetadata("counters", Section2);
		Configuration->AddMetadata("counters", "thirty-five", MakeShareable(Value));

		Configuration->AddOnBreadcrumb([](TSharedRef<IBugsnagBreadcrumb> Breadcrumb)
			{
				return true;
			});

		Configuration->AddOnSendError([](TSharedRef<IBugsnagEvent> Event)
			{
				Event->AddMetadata(TEXT("custom"), TEXT("configOnSendError"), TEXT("hello"));
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

		UBugsnagFunctionLibrary::AddOnSession([](TSharedRef<IBugsnagSession> Session)
			{
				return true;
			});

		UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("About to read from a bad memory address"));

		UBugsnagFunctionLibrary::AddFeatureFlag(TEXT("Bugsnag"));

		UBugsnagFunctionLibrary::ClearFeatureFlag(TEXT("nope"));

		UBugsnagFunctionLibrary::AddMetadata(TEXT("custom"), TEXT("someValue"), TEXT("foobar"));
		TSharedPtr<FJsonObject> CustomMetadata = UBugsnagFunctionLibrary::GetMetadata("custom");
		TSharedPtr<FJsonValue> ExistingValue = UBugsnagFunctionLibrary::GetMetadata("custom", "someValue");
		if (CustomMetadata.IsValid() && ExistingValue.IsValid() && CustomMetadata->HasField("someValue"))
		{
			UBugsnagFunctionLibrary::ClearMetadata("custom", "someValue");
			UBugsnagFunctionLibrary::AddMetadata("custom", "someOtherValue", ExistingValue);
		}

		FPlatformProcess::Sleep(0.5f); // Leave time for async breadcrumb / metadata I/O

		UBugsnagFunctionLibrary::SetContext("overhead view");

		UBugsnagFunctionLibrary::SetUser("5402", "usr@example.com", "");

		volatile int* Pointer = nullptr;
		*Pointer = 42;
	}
};

REGISTER_SCENARIO(BadMemoryAccessScenario);
