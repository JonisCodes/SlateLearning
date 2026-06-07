// Copyright Epic Games, Inc. All Rights Reserved.

using System.Reflection;
using UnrealBuildTool;

public class SlateLearning : ModuleRules
{
	public SlateLearning(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
			{ "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Slate", "SlateCore" });

		// PrivateDependencyModuleNames.AddRange(new string[]
		// {
		// });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate", "SlateCore", "CoreUObject", "Engine", "UMG"
		});

		PublicIncludePaths.AddRange(new string[]
		{
			ModuleDirectory + "/Public",
			ModuleDirectory + "/Public/Narrative",
			ModuleDirectory + "/Public/UI",
			ModuleDirectory + "/Public/UI/Structs",
			ModuleDirectory + "/Public/UI/UMG",
			ModuleDirectory + "/Public/Inventory",
			ModuleDirectory + "/Public/UI/RadialMenu",
		});

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}