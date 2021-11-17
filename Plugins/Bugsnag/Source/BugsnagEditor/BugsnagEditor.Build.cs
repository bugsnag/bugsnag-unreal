// Copyright 2021 Bugsnag. All Rights Reserved.

using UnrealBuildTool;

public class BugsnagEditor : ModuleRules
{
	public BugsnagEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		bEnforceIWYU = true;

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"UnrealEd",
				"Bugsnag",
			}
			);
	}
}
