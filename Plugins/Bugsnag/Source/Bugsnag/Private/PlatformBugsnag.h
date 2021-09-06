#pragma once

#include "CoreMinimal.h"

#if PLATFORM_IOS || PLATFORM_MAC
#define PLATFORM_IMPLEMENTS_BUGSNAG 1
#else
#define PLATFORM_IMPLEMENTS_BUGSNAG 0
#endif

#if PLATFORM_IMPLEMENTS_BUGSNAG
#include COMPILED_PLATFORM_HEADER(PlatformBugsnag.h)
#endif
