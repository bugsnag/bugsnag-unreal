#include "Misc/AutomationTest.h"

#include "Apple/AppleBugsnagUtils.h"

//
// Macros copied from Engine/Source/Developer/AutomationDriver/Private/Specs/AutomationDriver.spec.cpp
//
#define TEST_TRUE(expression) \
	EPIC_TEST_BOOLEAN_(TEXT(#expression), expression, true)

#define TEST_FALSE(expression) \
	EPIC_TEST_BOOLEAN_(TEXT(#expression), expression, false)

#define TEST_EQUAL(expression, expected) \
	EPIC_TEST_BOOLEAN_(TEXT(#expression), expression, expected)

#define EPIC_TEST_BOOLEAN_(text, expression, expected) \
	TestEqual(text, expression, expected);

#define TEST_EQUAL_OBJC(value, expected)                                    \
	if (![value isEqual:expected])                                          \
	{                                                                       \
		AddError(                                                           \
			FString::Printf(                                                \
				TEXT("Expected " #value " to be %s, but it was %s"),        \
				UTF8_TO_TCHAR(expected.description.UTF8String ?: "(null)"), \
				UTF8_TO_TCHAR(value.description.UTF8String ?: "(null)")),   \
			1);                                                             \
	}

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

	Describe("NSSetFromFStrings", [this]()
		{
			It("Converts from TArray<FString> to NSArray<NSString*>", [this]()
				{
					TEST_EQUAL_OBJC(NSSetFromFStrings({TEXT("one"), TEXT("two"), TEXT("three")}),
						([NSSet setWithArray:@[@"one", @"two", @"three"]]));
				});
		});
}
