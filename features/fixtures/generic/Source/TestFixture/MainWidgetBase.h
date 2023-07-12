// Copyright 2022 Bugsnag. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "MainWidgetBase.generated.h"

UCLASS()
class TESTFIXTURE_API UMainWidgetBase : public UUserWidget
{
	GENERATED_BODY()

	void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	void ExecuteMazeRunnerCommand();
	FString LoadMazeRunnerUrl();
	void RunMazeRunnerCommand(FString MazeRunnerBaseUrl);
};
