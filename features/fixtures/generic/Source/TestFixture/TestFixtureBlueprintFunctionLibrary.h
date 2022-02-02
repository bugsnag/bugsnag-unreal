#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TestFixtureBlueprintFunctionLibrary.generated.h"

UCLASS()
class TESTFIXTURE_API UTestFixtureBlueprintFunctionLibrary
	: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Maze Runner Fixture")
	static void AppendText(const FString& Text);

	UFUNCTION(BlueprintCallable, Category = "Maze Runner Fixture")
	static FString GetDisplayText();

	UFUNCTION(BlueprintCallable, Category = "Maze Runner Fixture")
	static void Run();

	UFUNCTION(BlueprintCallable, Category = "Maze Runner Fixture")
	static void Start();
};
