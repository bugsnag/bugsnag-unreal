#include "Misc/AutomationTest.h"

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
