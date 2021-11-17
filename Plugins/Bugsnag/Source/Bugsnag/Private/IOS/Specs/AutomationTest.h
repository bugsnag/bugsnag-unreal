// Copyright 2021 Bugsnag. All Rights Reserved.

#include "Misc/AutomationTest.h"

#define TEST_TRUE(expression) \
	TestTrue(TEXT(#expression), expression)

#define TEST_FALSE(expression) \
	TestFalse(TEXT(#expression), expression)

#define TEST_EQUAL(expression, expected) \
	TestEqual(TEXT(#expression), expression, expected)

#define TEST_EQUAL_OBJC(value, expected)                                      \
	if (![value isEqual:expected])                                            \
	{                                                                         \
		AddError(                                                             \
			FString::Printf(                                                  \
				TEXT("Expected " #value " to be %s, but it was %s"),          \
				UTF8_TO_TCHAR([expected description].UTF8String ?: "(null)"), \
				UTF8_TO_TCHAR([value description].UTF8String ?: "(null)")),   \
			1);                                                               \
	}
