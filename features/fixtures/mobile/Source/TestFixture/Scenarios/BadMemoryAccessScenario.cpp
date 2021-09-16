#include "Scenario.h"

class BadMemoryAccessScenario : public Scenario
{
public:
	void Run() override
	{
		// This doesn't really belong in this scenario, just checking that UBugsnagFunctionLibrary::GetBreadcrumbs() works
		TArray<TSharedPtr<const IBugsnagBreadcrumb>> Breadcrumbs = UBugsnagFunctionLibrary::GetBreadcrumbs();
		assert(Breadcrumbs.Num() > 0);
		assert(!Breadcrumbs[0].GetMessage().IsEmpty());

		UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("About to read from a bad memory address"));

		volatile int* Pointer = nullptr;
		*Pointer = 42;
	}
};

REGISTER_SCENARIO(BadMemoryAccessScenario);
