// Copyright 2021 Bugsnag. All Rights Reserved.

#include "ExampleBlueprintFunctionLibrary.h"

#include "BugsnagFunctionLibrary.h"

void UExampleBlueprintFunctionLibrary::InvalidMemoryAccess()
{
	volatile int* ptr = NULL;
	*ptr = 42;
}

void UExampleBlueprintFunctionLibrary::StackOverflow()
{
	TFunction<void()> foo = []
	{
		char buffer[300];
		snprintf(buffer, sizeof(buffer), "%s:%d", __FILE__, __LINE__);
		UExampleBlueprintFunctionLibrary::StackOverflow();
	};
	foo();
}

void UExampleBlueprintFunctionLibrary::Notify()
{
	UBugsnagFunctionLibrary::Notify(TEXT("Non-fatal"), TEXT("Notify() called from C++"));
}

//
// Unreal Engine disables C++ exceptions on non-desktop platforms, so
// for now there is no example throwing a C++ exception.
//
// See PLATFORM_EXCEPTIONS_DISABLED
//
