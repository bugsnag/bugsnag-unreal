#include "AutomationTest.h"

#include "Apple/AppleBugsnagUtils.h"

BEGIN_DEFINE_SPEC(FAppleBugsnagUtilsSpec, "Bugsnag.FAppleBugsnagUtilsSpec",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
END_DEFINE_SPEC(FAppleBugsnagUtilsSpec)
void FAppleBugsnagUtilsSpec::Define()
{
	Describe("FStringFromNSString", [this]()
		{
			It("Converts from NSString to FString", [this]()
				{
					TEST_EQUAL(FStringFromNSString(nil), TEXT(""));
					TEST_EQUAL(FStringFromNSString(@""), TEXT(""));
					TEST_EQUAL(FStringFromNSString(@"Hello, Unreal Engine!"), TEXT("Hello, Unreal Engine!"));
				});
		});

	Describe("NSStringFromFString", [this]()
		{
			It("Converts from FString to NSString", [this]()
				{
					TEST_EQUAL_OBJC(NSStringFromFString(TEXT("")), @"");
					TEST_EQUAL_OBJC(NSStringFromFString(TEXT("Hello, Unreal Engine!")), @"Hello, Unreal Engine!");
					TEST_EQUAL_OBJC(NSStringFromFString(FString()), @"");
				});
		});

	Describe("OptionalFromNSString", [this]()
		{
			It("Converts from NSString to TSharedPtr<FString>", [this]()
				{
					TEST_EQUAL(OptionalFromNSString(@"").GetValue(), TEXT(""));
					TEST_EQUAL(OptionalFromNSString(@"Hello, Unreal Engine!").GetValue(), TEXT("Hello, Unreal Engine!"));
				});
			It("Returns unset for nil", [this]()
				{
					TEST_FALSE(OptionalFromNSString(nil).IsSet());
				});
		});

	Describe("NSStringFromOptional", [this]()
		{
			It("Converts from TSharedPtr<FString> to NSString", [this]()
				{
					TEST_EQUAL_OBJC(NSStringFromOptional(FString(TEXT(""))), @"");
					TEST_EQUAL_OBJC(NSStringFromOptional(FString(TEXT("Hello, Unreal Engine!"))), @"Hello, Unreal Engine!");
					TEST_EQUAL_OBJC(NSStringFromOptional(FString(FString())), @"");
				});
			It("Returns nil for nullptr", [this]()
				{
					TEST_TRUE(NSStringFromOptional(TOptional<FString>()) == nil);
				});
		});

	Describe("FDateTimeFromNSDate", [this]()
		{
			It("Converts from NSDate to FDateTime", [this]()
				{
					time_t timestamp = time(NULL);
					TEST_EQUAL(FDateTimeFromNSDate([NSDate dateWithTimeIntervalSince1970:timestamp]),
						FDateTime::FromUnixTimestamp(timestamp));
				});
		});

	Describe("NSDateFromFDateTime", [this]()
		{
			It("Converts from FDateTime to NSDate", [this]()
				{
					time_t timestamp = time(NULL);
					TEST_EQUAL_OBJC(NSDateFromFDateTime(FDateTime::FromUnixTimestamp(timestamp)),
						[NSDate dateWithTimeIntervalSince1970:timestamp]);
				});
		});

	Describe("FJsonObjectFromNSDictionary", [this]()
		{
			It("Converts from NSDictionary to FJsonObject", [this]()
				{
					TSharedPtr<FJsonObject> JsonObject = FJsonObjectFromNSDictionary(@{@"foo": @"bar"});
					TEST_EQUAL(JsonObject->GetStringField(TEXT("foo")), TEXT("bar"));
				});

			It("Returns an invalid pointer if NSDictionary is not JSON convertible", [this]()
				{
					TEST_FALSE(FJsonObjectFromNSDictionary(@{@"date": [NSDate date]}).IsValid())
				});
		});

	Describe("NSDictionaryFromFJsonObject", [this]()
		{
			It("Converts from FJsonObject to NSDictionary", [this]()
				{
					TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
					JsonObject->SetStringField(TEXT("foo"), TEXT("bar"));
					TEST_EQUAL_OBJC(NSDictionaryFromFJsonObject(JsonObject), @{@"foo": @"bar"});
				});

			It("Returns nil when passed nullptr", [this]()
				{
					TEST_TRUE(NSDictionaryFromFJsonObject(nullptr) == nil);
				});
		});

	Describe("FJsonValueFromNSObject", [this]()
		{
			It("Converts from NSString to FJsonValueString", [this]()
				{
					TEST_TRUE(FJsonValue::CompareEqual(*FJsonValueFromNSObject(@"Testing"), FJsonValueString(TEXT("Testing"))));
				});

			It("Converts from NSValue to double", [this]()
				{
					TEST_TRUE(FJsonValue::CompareEqual(*FJsonValueFromNSObject(@3.141), FJsonValueNumber(3.141)));
				});

			It("Converts from NSValue to FJsonValueBoolean", [this]()
				{
					TEST_TRUE(FJsonValue::CompareEqual(*FJsonValueFromNSObject(@YES), FJsonValueBoolean(true)));
					TEST_TRUE(FJsonValue::CompareEqual(*FJsonValueFromNSObject(@NO), FJsonValueBoolean(false)));
				});

			It("Converts from NSArray to FJsonValueArray", [this]()
				{
					TEST_TRUE(FJsonValue::CompareEqual(*FJsonValueFromNSObject(@[]), FJsonValueArray({})));
				});

			It("Converts from NSDictionary to FJsonValueObject", [this]()
				{
					TEST_TRUE(FJsonValue::CompareEqual(*FJsonValueFromNSObject(@{}), FJsonValueObject(MakeShareable(new FJsonObject))));
				});

			It("Converts from NSNull to FJsonValueNull", [this]()
				{
					TEST_TRUE(FJsonValue::CompareEqual(*FJsonValueFromNSObject([NSNull null]), FJsonValueNull()));
				});

			It("Returns nullptr when passed nil", [this]()
				{
					TEST_TRUE(FJsonValueFromNSObject(nil) == nullptr);
				});
		});

	Describe("NSObjectFromFJsonValue", [this]()
		{
			It("Converts from FJsonValueString to NSString", [this]()
				{
					TEST_EQUAL_OBJC(NSObjectFromFJsonValue(MakeShareable(new FJsonValueString(TEXT("Testing")))), @"Testing");
				});

			It("Converts from FJsonValueNumber to NSValue", [this]()
				{
					TEST_EQUAL_OBJC(NSObjectFromFJsonValue(MakeShareable(new FJsonValueNumber(3.141))), @3.141);
				});

			It("Converts from FJsonValueBoolean to NSValue", [this]()
				{
					TEST_EQUAL_OBJC(NSObjectFromFJsonValue(MakeShareable(new FJsonValueBoolean(true))), @YES);
					TEST_EQUAL_OBJC(NSObjectFromFJsonValue(MakeShareable(new FJsonValueBoolean(false))), @NO);
				});

			It("Converts from FJsonValueArray to NSArray", [this]()
				{
					TEST_EQUAL_OBJC(NSObjectFromFJsonValue(MakeShareable(new FJsonValueArray({}))), @[]);
				});

			It("Converts from FJsonValueObject to NSDictionary", [this]()
				{
					TEST_EQUAL_OBJC(NSObjectFromFJsonValue(MakeShareable(new FJsonValueObject(MakeShareable(new FJsonObject)))), @{});
				});

			It("Converts from FJsonValueNull to NSNull", [this]()
				{
					TEST_EQUAL_OBJC(NSObjectFromFJsonValue(MakeShareable(new FJsonValueNull())), [NSNull null]);
				});

			It("Returns nil when passed nullptr", [this]()
				{
					TEST_TRUE(NSObjectFromFJsonValue(nullptr) == nil);
				});
		});

	Describe("NSSetFromFStrings", [this]()
		{
			It("Converts from TArray<FString> to NSArray<NSString*>", [this]()
				{
					TEST_EQUAL_OBJC(NSSetFromFStrings({TEXT("one"), TEXT("two"), TEXT("three")}),
						([NSSet setWithArray:@[@"one", @"two", @"three"]]));
				});
		});
}
