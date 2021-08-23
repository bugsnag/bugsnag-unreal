using UnrealBuildTool;
using System.Collections.Generic;

public class BugsnagExampleEditorTarget : TargetRules
{
	public BugsnagExampleEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "BugsnagExample" } );
	}
}
