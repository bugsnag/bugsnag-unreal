#include "Scenario.h"

class RedactedKeysScenario : public Scenario
{
public:
	void Configure() override
	{
		TArray<FString> RedactedKeys = Configuration->GetRedactedKeys();
		// RedactedKeys should contain "password" by default
		RedactedKeys.Add(TEXT("auth_key"));
		RedactedKeys.Add(TEXT("secret"));
		Configuration->SetRedactedKeys(RedactedKeys);
		Configuration->SetEnabledBreadcrumbTypes(EBugsnagEnabledBreadcrumbTypes::None);
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::AddOnBreadcrumb([](TSharedRef<IBugsnagBreadcrumb> Breadcrumb)
			{
				if (Breadcrumb->GetType() == EBugsnagBreadcrumbType::Manual)
				{
					TSharedRef<FJsonObject> Metadata = MakeShared<FJsonObject>();
					Metadata->SetStringField(TEXT("password"), TEXT("topsecret"));
					Metadata->SetStringField(TEXT("secret"), TEXT("You'll never guess"));
					Metadata->SetStringField(TEXT("foo"), TEXT("bar"));
					Breadcrumb->SetMetadata(Metadata);
				}
				return true;
			});

		UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("Whale hello there"));

		UBugsnagFunctionLibrary::AddMetadata(TEXT("custom"), TEXT("password"), TEXT("topsecret"));
		UBugsnagFunctionLibrary::AddMetadata(TEXT("custom"), TEXT("auth_key"), TEXT("123456"));
		UBugsnagFunctionLibrary::AddMetadata(TEXT("custom"), TEXT("text"), TEXT("This is not confidential"));

		UBugsnagFunctionLibrary::Notify(TEXT("Test"), TEXT("No message"));
	}
};

REGISTER_SCENARIO(RedactedKeysScenario)
