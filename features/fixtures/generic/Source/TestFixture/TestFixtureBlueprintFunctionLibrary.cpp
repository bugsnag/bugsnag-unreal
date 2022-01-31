#include "TestFixtureBlueprintFunctionLibrary.h"

#include "ScenarioNames.h"
#include "Scenarios/Scenario.h"

#include "Http.h"

static FString DisplayText;

static FString GetScenarioName()
{
	int32 ScenarioIndex = -1;
	LexFromString(ScenarioIndex, *DisplayText);
	UE_LOG(LogTestFixture, Display, TEXT("ScenarioIndex = %d"), ScenarioIndex);
	return ScenarioNames[ScenarioIndex];
}

void UTestFixtureBlueprintFunctionLibrary::AppendText(const FString& Text)
{
	DisplayText += Text;
}

FString UTestFixtureBlueprintFunctionLibrary::GetDisplayText()
{
	return DisplayText;
}

void UTestFixtureBlueprintFunctionLibrary::Run()
{
	FString ScenarioName = GetScenarioName();
	Scenario::Run(ScenarioName);
	DisplayText.Reset();
}

void UTestFixtureBlueprintFunctionLibrary::Start()
{
	FString ScenarioName = GetScenarioName();
	Scenario::Start(ScenarioName);
	DisplayText.Reset();
}
