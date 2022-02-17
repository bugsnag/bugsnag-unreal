using UnrealBuildTool;

public class TestFixtureTarget : TargetRules
{
	public TestFixtureTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		DefaultBuildSettings = BuildSettingsVersion.Latest;

		bEnforceIWYU = true;
		bUseUnityBuild = false;

		ExtraModuleNames.AddRange( new string[] { "TestFixture" } );
	}
}
