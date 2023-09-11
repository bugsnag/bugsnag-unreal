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

		if (Target.Platform == UnrealTargetPlatform.IOS || Target.Platform == UnrealTargetPlatform.Mac)
		{
			// Statically link to BugsnagCocoa
			PrivateDependencyModuleNames.Add("BugsnagCocoa");
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{

			string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
			#if UE_5_1_OR_LATER
				AdditionalPropertiesForReceipt.Add("AndroidPlugin", System.IO.Path.Combine(PluginPath, "Bugsnag_UPL_BAGP_V7.xml"));
			#else
				AdditionalPropertiesForReceipt.Add("AndroidPlugin", System.IO.Path.Combine(PluginPath, "Bugsnag_UPL.xml"));
			#endif
		}
	}
}
