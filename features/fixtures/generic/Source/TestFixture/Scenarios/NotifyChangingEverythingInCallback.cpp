#include "Scenario.h"

class NotifyChangingEverythingInCallback : public Scenario
{
public:
	void Configure() override
	{
		Configuration->AddMetadata("custom", "inConfigure", MakeShareable(new FJsonValueBoolean(true)));
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::Notify("Missing builder", "Task requires at least two", [](TSharedRef<IBugsnagEvent> Event)
			{
				// add metadata
				Event->AddMetadata("custom", "text", "some");
				Event->AddMetadata("custom", "number", 40);
				TSharedRef<FJsonObject> Custom = MakeShared<FJsonObject>();
				Custom->SetBoolField("missing", true);
				Event->AddMetadata("custom", Custom);

				// clear metadata
				Event->ClearMetadata("custom", "inConfigure");

				// app
				Event->GetApp()->SetIsLaunching(false);
				Event->GetApp()->SetIsInForeground(false);

				// device
				Event->GetDevice()->SetId(FString("51229"));
				Event->GetDevice()->SetJailbroken(true);

				// error
				Event->GetErrors()[0]->SetErrorClass("Why would you change this lol");
				Event->GetErrors()[0]->SetErrorMessage("Its literally a function argument");

				// event
				Event->SetContext(FString("changing lots"));
				Event->SetSeverity(EBugsnagSeverity::Info);
				Event->SetUnhandled(true);
				Event->SetUser(FString("319"));

				// thread
				auto Threads = Event->GetThreads();
				if (Threads.Num())
				{
					Threads[0]->SetId("9000");
					Threads[0]->SetName("Why would you change this either?");
				}

				return true;
			});
	}
};

REGISTER_SCENARIO(NotifyChangingEverythingInCallback);
