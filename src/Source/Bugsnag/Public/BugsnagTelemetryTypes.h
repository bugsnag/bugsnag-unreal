// Copyright 2022 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Types of telemetry that may be sent to Bugsnag for product improvement purposes.
 */
enum class EBugsnagTelemetryTypes : uint8
{
	None = 0,
	InternalErrors = 1 << 0,
	Usage = 1 << 1,
	All = InternalErrors | Usage
};
ENUM_CLASS_FLAGS(EBugsnagTelemetryTypes)
