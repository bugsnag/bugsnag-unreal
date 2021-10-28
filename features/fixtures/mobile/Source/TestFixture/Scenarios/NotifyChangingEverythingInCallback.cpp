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
				Event->AddMetadata("custom", "text", MakeShareable(new FJsonValueString("some")));
				Event->AddMetadata("custom", "number", MakeShareable(new FJsonValueNumber(40)));
				TSharedRef<FJsonObject> Custom = MakeShared<FJsonObject>();
				Custom->SetBoolField("missing", true);
				Event->AddMetadata("custom", Custom);

				// clear metadata
				Event->ClearMetadata("custom", "inConfigure");

				// app
				Event->GetApp()->SetIsLaunching(MakeShareable(new bool(false)));
				Event->GetApp()->SetIsInForeground(MakeShareable(new bool(false)));

				// device
				Event->GetDevice()->SetId(MakeShareable(new FString("51229")));
				Event->GetDevice()->SetJailbroken(MakeShareable(new bool(true)));

				// error
				Event->GetErrors()[0]->SetErrorClass("Why would you change this lol");
				Event->GetErrors()[0]->SetErrorMessage("Its literally a function argument");

				// event
				Event->SetContext(MakeShareable(new FString("changing lots")));
				Event->SetSeverity(EBugsnagSeverity::Info);
				Event->SetUnhandled(true);
				Event->SetUser(MakeShareable(new FString("319")));

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
