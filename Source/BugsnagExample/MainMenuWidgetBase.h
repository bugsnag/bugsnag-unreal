// Copyright 2022 Bugsnag. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenuWidgetBase.generated.h"

UCLASS()
class BUGSNAGEXAMPLE_API UMainMenuWidgetBase : public UUserWidget
{
	GENERATED_BODY()

	void NativeOnInitialized() override
	{
		UUserWidget::NativeOnInitialized();
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
	}
};
