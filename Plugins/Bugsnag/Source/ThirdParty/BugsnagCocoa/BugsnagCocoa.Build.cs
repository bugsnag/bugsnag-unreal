// Copyright 2021 Bugsnag. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

public class BugsnagCocoa : ModuleRules
{
	public BugsnagCocoa(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External; // prevents UBT from building all sources it find in the module

		string IncludePath = Path.Combine(ModuleDirectory, "include");
		CheckFileExists(Path.Combine(IncludePath, "Bugsnag", "Bugsnag.h"));
		CheckFileExists(Path.Combine(IncludePath, "BugsnagPrivate", "Bugsnag+Private.h"));
		PublicSystemIncludePaths.Add(IncludePath);

		string LibraryPath = Path.Combine(ModuleDirectory, Target.Platform.ToString(), "libBugsnagStatic.a");
		CheckFileExists(LibraryPath);
		PublicAdditionalLibraries.Add(LibraryPath);
	}

	private void CheckFileExists(string Path)
	{
		if (!File.Exists(Path))
		{
			throw new BuildException("Missing file: " + Path);
		}
	}
}
