using UnrealBuildTool;
using System.IO;

public class BugsnagCocoa : ModuleRules
{
	public BugsnagCocoa(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External; // prevents UBT from building all sources it find in the module

		PublicSystemIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));

		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, Target.Platform.ToString(), "Release", "libBugsnagStatic.a"));
	}
}
