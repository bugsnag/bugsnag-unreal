#pragma once

#include "CoreMinimal.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/LargeMemoryReader.h"
#include "Serialization/LargeMemoryWriter.h"

#import <BugsnagPrivate/BSGJSONSerialization.h>

static inline FString FStringFromNSString(NSString* _Nullable String)
{
	return String ? FString(UTF8_TO_TCHAR(String.UTF8String)) : TEXT("");
}

static inline NSString* NSStringFromFString(const FString& String)
{
	return @(TCHAR_TO_UTF8(*String));
}

static inline FDateTime FDateTimeFromNSDate(NSDate* Date)
{
	return FDateTime(FDateTime(1970, 1, 1).GetTicks() + Date.timeIntervalSince1970 * ETimespan::TicksPerSecond);
}

static inline NSDate* NSDateFromFDateTime(const FDateTime& DateTime)
{
	return [NSDate dateWithTimeIntervalSince1970:(DateTime.GetTicks() - FDateTime(1970, 1, 1).GetTicks()) / ETimespan::TicksPerSecond];
}

static inline TSharedPtr<FJsonObject> FJsonObjectFromNSDictionary(NSDictionary* Dictionary, NSError** Error = nil)
{
	NSData* Data = [BSGJSONSerialization dataWithJSONObject:Dictionary options:0 error:Error];
	if (Data)
	{
		TSharedPtr<FJsonObject> JsonObject;
		FLargeMemoryReader Archive((const uint8*)Data.bytes, Data.length);
		if (FJsonSerializer::Deserialize(TJsonReaderFactory<char>::Create(&Archive), JsonObject))
		{
			return JsonObject;
		}
	}
	return nullptr;
}

static inline NSDictionary* _Nullable NSDictionaryFromFJsonObject(const TSharedPtr<FJsonObject>& JsonObject, NSError** Error = nil)
{
	if (!JsonObject.IsValid())
	{
		return nil;
	}
	FLargeMemoryWriter Archive;
	TSharedRef<TJsonWriter<char>> JsonWriter = TJsonWriterFactory<char>::Create(&Archive);
	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter))
	{
		JsonWriter->Close();
		NSData* Data = [NSData dataWithBytesNoCopy:Archive.GetData() length:Archive.TotalSize() freeWhenDone:NO];
		return [BSGJSONSerialization JSONObjectWithData:Data options:0 error:Error];
	}
	return nil;
}

static inline NSSet* NSSetFromFStrings(const TArray<FString>& Array)
{
	NSMutableSet* Set = [NSMutableSet set];
	for (const FString& Value : Array)
	{
		[Set addObject:NSStringFromFString(Value)];
	}
	return Set;
}
