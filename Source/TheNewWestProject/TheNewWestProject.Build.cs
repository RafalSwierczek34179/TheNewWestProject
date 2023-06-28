// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TheNewWestProject : ModuleRules
{
	public TheNewWestProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
