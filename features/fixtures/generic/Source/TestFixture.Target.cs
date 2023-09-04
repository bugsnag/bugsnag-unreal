using UnrealBuildTool;

public class TestFixtureTarget : TargetRules
{
	public TestFixtureTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		bEnforceIWYU = true;
		bUseUnityBuild = true;

		ExtraModuleNames.AddRange( new string[] { "TestFixture" } );
	}
}
