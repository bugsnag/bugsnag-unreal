// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "BugsnagConfiguration.h"

@class BugsnagConfiguration;

class FApplePlatformConfiguration
{
public:
	static BugsnagConfiguration* Configuration(const TSharedRef<FBugsnagConfiguration>& Configuration);
};
