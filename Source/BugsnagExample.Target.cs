using UnrealBuildTool;
using System.Collections.Generic;

public class BugsnagExampleTarget : TargetRules
{
	public BugsnagExampleTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "BugsnagExample" } );
	}
}
