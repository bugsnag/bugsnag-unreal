using UnrealBuildTool;
using System.Collections.Generic;

public class BugsnagExampleTarget : TargetRules
{
	public BugsnagExampleTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		bEnforceIWYU = true;
		bUseUnityBuild = false;

		ExtraModuleNames.AddRange( new string[] { "BugsnagExample" } );
	}
}
