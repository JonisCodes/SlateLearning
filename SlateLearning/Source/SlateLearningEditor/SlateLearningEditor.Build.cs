using UnrealBuildTool;

public class SlateLearningEditor : ModuleRules
{
	public SlateLearningEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"SlateLearning",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"UnrealEd",
			"LevelEditor",
			"EditorStyle",
			"AssetTools",
			"ToolMenus",
			"EditorFramework",
			"BlueprintGraph",
			"GraphEditor",
			"ApplicationCore",
			"InputCore",
			"DesktopPlatform",
		});
		
		PublicIncludePaths.AddRange(new string[]
		{
			ModuleDirectory + "/Public",
			ModuleDirectory + "/Public/Narrative"
		});
	}
}