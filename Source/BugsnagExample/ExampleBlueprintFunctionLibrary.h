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
	static void StartFromCode(const FString& ApiKey);

	UFUNCTION(BlueprintCallable, Category = "BugsnagExample")
	static void InvalidMemoryAccess();

	UFUNCTION(BlueprintCallable, Category = "BugsnagExample")
	static void StackOverflow();

	UFUNCTION(BlueprintCallable, Category = "BugsnagExample")
	static void Notify();

	UFUNCTION(BlueprintCallable, Category = "BugsnagExample")
	static const FString GetApiKeyFromConfig();

	UFUNCTION(BlueprintCallable, Category = "BugsnagExample")
	static void OneThousandBreadcrumbs();

	UFUNCTION(BlueprintCallable, Category = "BugsnagExample")
	static void iOSException();
};
