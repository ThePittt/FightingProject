// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GEII_FightingProject : ModuleRules
{
	public GEII_FightingProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",});
	}
}
