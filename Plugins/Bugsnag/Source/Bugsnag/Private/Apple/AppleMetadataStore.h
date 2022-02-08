// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "AppleBugsnagUtils.h"
#include "BugsnagMetadataStore.h"

#import <Bugsnag/BugsnagMetadataStore.h>

class FAppleMetadataStore : public virtual IBugsnagMetadataStore
{
public:
	FAppleMetadataStore(id<BugsnagMetadataStore> CocoaStore = nil)
		: CocoaStore(CocoaStore)
	{
	}

	using IBugsnagMetadataStore::AddMetadata;

	void AddMetadata(const FString& Section, const TSharedRef<FJsonObject>& Metadata) override
	{
		[CocoaStore addMetadata:NSDictionaryFromFJsonObject(Metadata) toSection:NSStringFromFString(Section)];
	}

	void AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value) override
	{
		[CocoaStore addMetadata:NSObjectFromFJsonValue(Value) withKey:NSStringFromFString(Key) toSection:NSStringFromFString(Section)];
	}

	TSharedPtr<FJsonObject> GetMetadata(const FString& Section) override
	{
		NSDictionary* Metadata = [CocoaStore getMetadataFromSection:NSStringFromFString(Section)];
		return [Metadata isKindOfClass:[NSDictionary class]] ? FJsonObjectFromNSDictionary(Metadata) : nullptr;
	}

	TSharedPtr<FJsonValue> GetMetadata(const FString& Section, const FString& Key) override
	{
		return FJsonValueFromNSObject([CocoaStore getMetadataFromSection:NSStringFromFString(Section) withKey:NSStringFromFString(Key)]);
	}

	void ClearMetadata(const FString& Section) override
	{
		[CocoaStore clearMetadataFromSection:NSStringFromFString(Section)];
	}

	void ClearMetadata(const FString& Section, const FString& Key) override
	{
		[CocoaStore clearMetadataFromSection:NSStringFromFString(Section) withKey:NSStringFromFString(Key)];
	}

	id<BugsnagMetadataStore> CocoaStore;
};
