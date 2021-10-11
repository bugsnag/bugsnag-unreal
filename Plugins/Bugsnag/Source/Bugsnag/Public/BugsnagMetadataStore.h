#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

class BUGSNAG_API IBugsnagMetadataStore
{
public:
	virtual void AddMetadata(const FString& Section, const TSharedPtr<FJsonObject>& Metadata) = 0;

	virtual void AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value) = 0;

	virtual TSharedPtr<FJsonObject> GetMetadata(const FString& Section) = 0;

	virtual TSharedPtr<FJsonValue> GetMetadata(const FString& Section, const FString& Key) = 0;

	virtual void ClearMetadata(const FString& Section) = 0;

	virtual void ClearMetadata(const FString& Section, const FString& Key) = 0;
};
