#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ExampleBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UExampleBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "BugsnagExample")
	static void InvalidMemoryAccess();

	UFUNCTION(BlueprintCallable, Category = "BugsnagExample")
	static void StackOverflow();
};
