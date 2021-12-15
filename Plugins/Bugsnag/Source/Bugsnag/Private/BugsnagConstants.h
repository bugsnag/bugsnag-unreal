// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace BugsnagConstants
{
extern const TCHAR* const Activity;
extern const TCHAR* const Device;
extern const TCHAR* const GameStateName;
extern const TCHAR* const GPUAdapterName;
extern const TCHAR* const GPUDriverVersion;
extern const TCHAR* const MapUrl;
extern const TCHAR* const Name;
extern const TCHAR* const UnrealEngine;
extern const TCHAR* const Url;
extern const TCHAR* const UserActivity;
extern const TCHAR* const Version;
}; // namespace BugsnagConstants

namespace BugsnagBreadcrumbMessages
{
extern const TCHAR* const GameStateChanged;
extern const TCHAR* const MapLoadFailed;
extern const TCHAR* const MapLoaded;
extern const TCHAR* const MapLoading;
extern const TCHAR* const UserActivityChanged;
}; // namespace BugsnagBreadcrumbMessages
