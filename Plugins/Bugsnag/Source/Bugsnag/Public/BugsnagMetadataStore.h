// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

class BUGSNAG_API IBugsnagMetadataStore
{
public:
	virtual void AddMetadata(const FString& Section, const TSharedRef<FJsonObject>& Metadata) = 0;

	virtual void AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value) = 0;

	void AddMetadata(const FString& Section, const FString& Key, const FString& Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueString>(Value));
	}

	void AddMetadata(const FString& Section, const FString& Key, const TCHAR* Value)
	{
		AddMetadata(Section, Key, MakeShareable(Value ? new FJsonValueString(Value) : nullptr));
	}

	void AddMetadata(const FString& Section, const FString& Key, const char* Value)
	{
		AddMetadata(Section, Key, MakeShareable(Value ? new FJsonValueString(Value) : nullptr));
	}

	void AddMetadata(const FString& Section, const FString& Key, double Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	void AddMetadata(const FString& Section, const FString& Key, float Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	void AddMetadata(const FString& Section, const FString& Key, int8 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	void AddMetadata(const FString& Section, const FString& Key, int16 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	void AddMetadata(const FString& Section, const FString& Key, int32 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	void AddMetadata(const FString& Section, const FString& Key, int64 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	void AddMetadata(const FString& Section, const FString& Key, uint8 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	void AddMetadata(const FString& Section, const FString& Key, uint16 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	void AddMetadata(const FString& Section, const FString& Key, uint32 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	void AddMetadata(const FString& Section, const FString& Key, uint64 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	void AddMetadata(const FString& Section, const FString& Key, bool Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueBoolean>(Value));
	}

	virtual TSharedPtr<FJsonObject> GetMetadata(const FString& Section) = 0;

	virtual TSharedPtr<FJsonValue> GetMetadata(const FString& Section, const FString& Key) = 0;

	virtual void ClearMetadata(const FString& Section) = 0;

	virtual void ClearMetadata(const FString& Section, const FString& Key) = 0;
};
