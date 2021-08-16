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
				"Engine"
			}
			);

		if (Target.Platform == UnrealTargetPlatform.IOS || Target.Platform == UnrealTargetPlatform.Mac)
		{
			// Statically link to BugsnagCocoa
			PrivateDependencyModuleNames.Add("BugsnagCocoa");
		}
	}
}
