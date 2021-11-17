// Copyright 2021 Bugsnag. All Rights Reserved.

using UnrealBuildTool;

public class Bugsnag : ModuleRules
{
	public Bugsnag(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		bEnforceIWYU = true;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"Json",
				"RHI"
			}
			);

		if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			// Statically link to BugsnagCocoa
			PrivateDependencyModuleNames.Add("BugsnagCocoa");
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
			AdditionalPropertiesForReceipt.Add("AndroidPlugin", System.IO.Path.Combine(PluginPath, "Bugsnag_UPL.xml"));
		}
	}
}
