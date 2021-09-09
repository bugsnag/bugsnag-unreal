#pragma once

#include "BugsnagConfiguration.h"

@class BugsnagConfiguration;

class FApplePlatformConfiguration
{
public:
	static BugsnagConfiguration* Configuration(const TSharedPtr<FBugsnagConfiguration>& Configuration);
};
