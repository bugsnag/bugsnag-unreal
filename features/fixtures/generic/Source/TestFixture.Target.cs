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

        if (Target.Platform == UnrealTargetPlatform.IOS ||
            Target.Platform == UnrealTargetPlatform.Mac ||
            Target.Platform == UnrealTargetPlatform.Linux)
        {
            // Correct: append to string with spaces
            AdditionalCompilerArguments += " -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer";
        }
    }
}
