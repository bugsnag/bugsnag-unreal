using UnrealBuildTool;

public class TestFixtureEditorTarget : TargetRules
{
	public TestFixtureEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		DefaultBuildSettings = BuildSettingsVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "TestFixture" } );
	}
}
