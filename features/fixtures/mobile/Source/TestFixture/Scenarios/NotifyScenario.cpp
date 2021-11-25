#include "Scenario.h"

class NotifyScenario : public Scenario
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

		Configuration->AddMetadata("pastries", Section1);
		Configuration->AddMetadata("counters", Section2);
		Configuration->AddMetadata("counters", "thirty-five", MakeShareable(Value));

		Configuration->AddOnBreadcrumb([](TSharedRef<IBugsnagBreadcrumb> Breadcrumb)
			{
				return true;
			});

		Configuration->AddOnSendError([](TSharedRef<IBugsnagEvent> Event)
			{
				// Intentionally passing a char* value to check it isn't implicitly casted to a non-string type
				Event->AddMetadata(TEXT("custom"), TEXT("configOnSendError"), "hello");
				return true;
			});

		Configuration->AddOnSession([](TSharedRef<IBugsnagSession> Session)
			{
				return true;
			});

		// sent in event payload
		Configuration->SetProjectPackages({TEXT("com.example.package")});
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

		UBugsnagFunctionLibrary::SetContext("pause menu");

		TSharedRef<FJsonObject> AdditionalValues = MakeShared<FJsonObject>();
		AdditionalValues->SetStringField("someValue", "foobar");
		AdditionalValues->SetBoolField("lastValue", true);
		UBugsnagFunctionLibrary::AddMetadata("custom", AdditionalValues);

		UBugsnagFunctionLibrary::SetUser("1118", "emilie@example.com", "Emilie");

		UBugsnagFunctionLibrary::Notify(TEXT("Internal Error"), TEXT("Does not compute"), [](TSharedRef<IBugsnagEvent> Event)
			{
				Event->AddMetadata(TEXT("custom"), TEXT("notify"), TEXT("testing"));
				Event->GetDevice()->SetId(FString("51229"));
				Event->GetApp()->SetDuration(37);
				FString ErrorClass = Event->GetErrors()[0]->GetErrorClass();
				Event->GetErrors()[0]->SetErrorClass(ErrorClass + TEXT(" happened"));
				return true;
			});
	}
};

REGISTER_SCENARIO(NotifyScenario);
