using UnrealBuildTool;
using System.Collections.Generic;

public class TestFixtureTarget : TargetRules
{
    public TestFixtureTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        bUseUnityBuild = true;
        ExtraModuleNames.AddRange(new string[] { "TestFixture" });

        // Allow modifying build environment for this target
        bOverrideBuildEnvironment = true;

        if (Target.Platform == UnrealTargetPlatform.IOS ||
            Target.Platform == UnrealTargetPlatform.Mac ||
            Target.Platform == UnrealTargetPlatform.Linux)
        {
            // Append Clang flags (string, not List)
            AdditionalCompilerArguments += " -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer";
        }
    }
}
