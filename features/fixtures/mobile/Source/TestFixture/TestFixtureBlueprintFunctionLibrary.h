#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TestFixtureBlueprintFunctionLibrary.generated.h"

UCLASS()
class TESTFIXTURE_API UTestFixtureBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Text Fixture")
	static void OnTextChanged(const FText& Text);
};
