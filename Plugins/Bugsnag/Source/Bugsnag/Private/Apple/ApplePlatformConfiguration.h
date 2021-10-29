#pragma once

#include "BugsnagConfiguration.h"

@class BugsnagConfiguration;

class FApplePlatformConfiguration
{
public:
	static BugsnagConfiguration* Configuration(const TSharedRef<FBugsnagConfiguration>& Configuration);
};
