// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_ANDROID
#include COMPILED_PLATFORM_HEADER(PlatformBugsnag.h)
#define PLATFORM_IMPLEMENTS_BUGSNAG 1

#elif PLATFORM_IOS || PLATFORM_MAC
#include "Apple/ApplePlatformBugsnag.h"
#define PLATFORM_IMPLEMENTS_BUGSNAG 1

#else
#define PLATFORM_IMPLEMENTS_BUGSNAG 0
#endif
