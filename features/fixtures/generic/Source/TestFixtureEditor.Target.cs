using UnrealBuildTool;

public class TestFixtureEditorTarget : TargetRules
{
	public TestFixtureEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		DefaultBuildSettings = BuildSettingsVersion.Latest;

		bEnforceIWYU = true;
		bUseUnityBuild = true;

		ExtraModuleNames.AddRange( new string[] { "TestFixture" } );
	}
}
