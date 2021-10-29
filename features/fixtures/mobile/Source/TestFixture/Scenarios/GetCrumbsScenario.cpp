#include "Scenario.h"
#include <cstdlib>

class GetCrumbScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->AddOnBreadcrumb([](TSharedRef<IBugsnagBreadcrumb> Crumb)
			{
				return Crumb->GetType() == EBugsnagBreadcrumbType::User;
			});
	}

	void Run() override
	{
		UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("Introspected?"), nullptr, EBugsnagBreadcrumbType::User);
		FPlatformProcess::Sleep(0.5f); // Leave time for async breadcrumb I/O
		auto Breadcrumbs = UBugsnagFunctionLibrary::GetBreadcrumbs();
		if (Breadcrumbs.Num() > 0										 //
			&& Breadcrumbs[0]->GetType() == EBugsnagBreadcrumbType::User //
			&& Breadcrumbs[0]->GetMessage().Compare("Introspected?", ESearchCase::CaseSensitive) == 0)
		{
			UBugsnagFunctionLibrary::Notify("It worked?", "");
		}
		else
		{
			abort(); // fail with style
		}
	}
};

REGISTER_SCENARIO(GetCrumbScenario)
