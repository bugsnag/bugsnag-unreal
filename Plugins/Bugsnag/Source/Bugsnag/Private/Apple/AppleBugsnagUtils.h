#pragma once

#include "CoreMinimal.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/LargeMemoryReader.h"
#include "Serialization/LargeMemoryWriter.h"

#import <BugsnagPrivate/BSGJSONSerialization.h>

// String conversion

static inline FString FStringFromNSString(NSString* _Nullable String)
{
	return String ? FString(UTF8_TO_TCHAR(String.UTF8String)) : TEXT("");
}

static inline NSString* _Nonnull NSStringFromFString(const FString& String)
{
	return @(TCHAR_TO_UTF8(*String));
}

static inline TSharedPtr<FString> FStringPtrFromNSString(NSString* _Nullable String)
{
	return String ? MakeShareable(new FString(UTF8_TO_TCHAR(String.UTF8String))) : nullptr;
}

static inline NSString* _Nullable NSStringFromFStringPtr(const TSharedPtr<FString>& String)
{
	return String.IsValid() ? @(TCHAR_TO_UTF8(**String)) : nil;
}

// Date conversion

static inline FDateTime FDateTimeFromNSDate(NSDate* Date)
{
	return FDateTime(FDateTime(1970, 1, 1).GetTicks() + Date.timeIntervalSince1970 * ETimespan::TicksPerSecond);
}

static inline NSDate* NSDateFromFDateTime(const FDateTime& DateTime)
{
	return [NSDate dateWithTimeIntervalSince1970:(DateTime.GetTicks() - FDateTime(1970, 1, 1).GetTicks()) / ETimespan::TicksPerSecond];
}

// Number conversion

static inline TSharedPtr<uint64> UInt64PtrFromNSNumber(NSNumber* _Nullable Number)
{
	return Number ? MakeShareable(new uint64(Number.unsignedLongValue)) : nullptr;
}

// JSON object conversion

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

// JSON value conversion

static inline TSharedPtr<FJsonValue> FJsonValueFromNSObject(id Value, NSError** Error = nil)
{
	if (!Value)
	{
		return nullptr;
	}
	TSharedPtr<FJsonObject> JsonObject = FJsonObjectFromNSDictionary(@{@"Value": Value}, Error);
	return JsonObject.IsValid() ? JsonObject->TryGetField(TEXT("Value")) : nullptr;
}

static inline id _Nullable NSObjectFromFJsonValue(const TSharedPtr<FJsonValue>& JsonValue, NSError** Error = nil)
{
	if (!JsonValue.IsValid())
	{
		return nil;
	}
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetField(TEXT("Value"), JsonValue);
	return NSDictionaryFromFJsonObject(JsonObject, Error)[@"Value"];
}

// Misc

static inline NSSet* NSSetFromFStrings(const TArray<FString>& Array)
{
	NSMutableSet* Set = [NSMutableSet set];
	for (const FString& Value : Array)
	{
		[Set addObject:NSStringFromFString(Value)];
	}
	return Set;
}
