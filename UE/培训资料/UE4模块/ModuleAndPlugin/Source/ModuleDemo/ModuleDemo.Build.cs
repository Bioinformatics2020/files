// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ModuleDemo : ModuleRules
{
	public ModuleDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
	}
}
