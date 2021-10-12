#include "Scenario.h"

class ModifyCrumbsScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->AddOnBreadcrumb([](TSharedRef<IBugsnagBreadcrumb> Crumb)
			{
				FString Message = Crumb->GetMessage();
				TSharedPtr<FJsonObject> Metadata = Crumb->GetMetadata();
				if (Crumb->GetType() == EBugsnagBreadcrumbType::User && Metadata->HasField("forty") && Metadata->HasField("macaron") && Message.Compare("Crash time", ESearchCase::CaseSensitive) == 0)
				// HasField returns false when the value is false
				{
					Metadata->SetStringField("forty", "four zero");
					Crumb->SetMetadata(Metadata);
					Crumb->SetMessage(Message + TEXT(" now"));
					Crumb->SetType(EBugsnagBreadcrumbType::Request);
					return true;
				}
				return false;
			});
	}

	void Run() override
	{
		TSharedPtr<FJsonObject> Metadata = MakeShared<FJsonObject>();
		Metadata->SetBoolField("cronut", false);
		Metadata->SetNumberField("macaron", 3);
		Metadata->SetStringField("forty", TEXT("40"));

		UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("Crash time"),
			Metadata, EBugsnagBreadcrumbType::User);
		FPlatformProcess::Sleep(0.5f); // Leave time for async breadcrumb I/O

		UBugsnagFunctionLibrary::Notify("Incorrect", "Missing token");
	}
};

REGISTER_SCENARIO(ModifyCrumbsScenario);
