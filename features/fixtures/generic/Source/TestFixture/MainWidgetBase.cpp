// Copyright 2022 Bugsnag. All Rights Reserved.

#include "MainWidgetBase.h"

#include "CoreMinimal.h"
#include "Http.h"
#include "Json.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/LargeMemoryReader.h"

#include "LogTestFixture.h"
#include "ScenarioNames.h"
#include "Scenarios/Scenario.h"

#if PLATFORM_ANDROID || PLATFORM_IOS
#define MAZE_RUNNER_URL_BASE "http://bs-local.com:9339"
#else
#define MAZE_RUNNER_URL_BASE "http://localhost:9339"
#endif

#if PLATFORM_ANDROID
#define CONFIG_PATH "fixture_config.json"
#else
#define CONFIG_PATH "fixture_config.json"
#endif

void UMainWidgetBase::NativeOnInitialized()
{
	UUserWidget::NativeOnInitialized();

#if PLATFORM_MAC
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);

	if (FApp::IsGame())
	{
		// Wait for map loading to prevent unexpected breadcrumbs when running scenarios.
		FCoreUObjectDelegates::PostLoadMapWithWorld.AddLambda([this](UWorld* World)
			{
				static bool once;
				if (!once)
				{
					once = true;
					ExecuteMazeRunnerCommand(MAZE_RUNNER_URL_BASE);
				}
			});
	}
#endif
}

FString UMainWidgetBase::LoadMazeRunnerUrl()
{
	FString RawConfig;
	FFileHelper::LoadFileToString(RawConfig, CONFIG_PATH);
	TSharedPtr<FJsonObject> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(RawConfig);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		FString& MazeAddress = JsonParsed->GetStringField("maze_address");
		UE_LOG(LogTestFixture, Display, TEXT("LoadMazeRunnerUrl: Loaded %s"), MazeAddress);
		return MazeAddress;
	}
	else
	{
		// Fail in some way, this fills in for now
		UE_LOG(LogTestFixture, Error, TEXT("LoadMazeRunnerUrl: Couldn't load maze_url, got %s"), RawConfig);
		return "";
	}

}

// A temporary name as I can't get the test fixture to open locally to change the button call
void UMainWidgetBase::ExecuteMazeRunnerCommand()
{
	RunMazeRunnerCommand(LoadMazeRunnerUrl());
}

void UMainWidgetBase::RunMazeRunnerCommand(FString MazeRunnerBaseUrl)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("GET");
	HttpRequest->SetURL(TEXT(MazeRunnerBaseUrl "/command"));
	HttpRequest->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bConnectedSuccessfully)
		{
			if (!bConnectedSuccessfully || !HttpResponse.IsValid())
			{
				UE_LOG(LogTestFixture, Error, TEXT("ExecuteMazeRunnerCommand: no response"));
				return;
			}

			int32 StatusCode = HttpResponse->GetResponseCode();
			if (StatusCode != 200)
			{
				UE_LOG(LogTestFixture, Error, TEXT("ExecuteMazeRunnerCommand: status code %d"), StatusCode);
				return;
			}

			UE_LOG(LogTestFixture, Display, TEXT("ExecuteMazeRunnerCommand: %s"), *(HttpResponse->GetContentAsString()));

			TSharedPtr<FJsonObject> JsonObject;
			const TArray<uint8>& Content = HttpResponse->GetContent();
			FLargeMemoryReader Archive((const uint8*)Content.GetData(), Content.Num());
			if (!FJsonSerializer::Deserialize(TJsonReaderFactory<char>::Create(&Archive), JsonObject))
			{
				UE_LOG(LogTestFixture, Error, TEXT("ExecuteMazeRunnerCommand: Could not deserialize Maze Runner command"));
				return;
			}

			const FString& Action = JsonObject->GetStringField(TEXT("action"));
			const FString& ScenarioName = JsonObject->GetStringField(TEXT("scenario_name"));

			if (Action == TEXT("run_scenario"))
			{
				Scenario::Run(ScenarioName);
			}
			else if (Action == TEXT("start_bugsnag"))
			{
				Scenario::Start(ScenarioName);
			}
			else
			{
				UE_LOG(LogTestFixture, Error, TEXT("ExecuteMazeRunnerCommand: bad action: \"%s\""), *Action);
			}
		});
	HttpRequest->ProcessRequest();
}
