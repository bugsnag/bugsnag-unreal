#include "Scenario.h"

class ModifySessionScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->SetAutoTrackSessions(false);
		Configuration->AddOnSession([](TSharedRef<IBugsnagSession> Session)
			{
				Session->SetId(TEXT("4a"));
				Session->SetStartedAt(FDateTime(1976, 1, 3));
				Session->GetApp()->SetType(MakeShareable(new FString("variant 2")));
				Session->GetApp()->SetVersionCode(21);
				Session->GetDevice()->SetOsName(MakeShareable(new FString("Fun OS")));
				Session->GetDevice()->SetOsVersion(MakeShareable(new FString("3.1")));
				TSharedPtr<TArray<FString>> ABIs = Session->GetDevice()->GetCpuAbi();
				if (ABIs.IsValid())
				{
					ABIs->Empty();
					ABIs->Add(TEXT("beanmachine"));
					Session->GetDevice()->SetCpuAbi(ABIs);
				}
				TSharedPtr<TMap<FString, FString>> Versions = Session->GetDevice()->GetRuntimeVersions();
				Versions->Add("libpastry", "4.5.0.0");
				Session->GetDevice()->SetRuntimeVersions(Versions);
				return true;
			});
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::StartSession();
		UBugsnagFunctionLibrary::Notify("Incorrect", "Missing token");
	}
};

REGISTER_SCENARIO(ModifySessionScenario);
