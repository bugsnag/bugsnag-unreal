// Copyright 2021 Bugsnag. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BugsnagExampleEditorTarget : TargetRules
{
	public BugsnagExampleEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		bEnforceIWYU = true;
		bUseUnityBuild = false;

		ExtraModuleNames.AddRange( new string[] { "BugsnagExample" } );
	}
}
