// Copyright 2022 Bugsnag. All Rights Reserved.

#include "Misc/AutomationTest.h"

#define TEST_TRUE(expression) \
	TestTrue(TEXT(#expression), expression)

#define TEST_FALSE(expression) \
	TestFalse(TEXT(#expression), expression)

#define TEST_EQUAL(expression, expected) \
	TestEqual(TEXT(#expression), expression, expected)
