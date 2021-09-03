using UnrealBuildTool;

public class TestFixture : ModuleRules
{
	public TestFixture(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "SlateCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Bugsnag" });
	}
}
