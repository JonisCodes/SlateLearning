#include "SlateLearningEditor.h"

#include "AssetToolsModule.h"
#include "EdGraphUtilities.h"
#include "SStatInspectorPanel.h"
#include "FNarrativeGraphAssetTypeActions.h"
#include "FNarrativeGraphEditorCommands.h"
#include "FNarrativeGraphNodeFactory.h"

class IAssetTools;
IMPLEMENT_MODULE(FSlateLearningEditorModule, SlateLearningEditor)

void FSlateLearningEditorModule::StartupModule()
{
	FNarrativeGraphEditorCommands::Register();
	
	NodeFactory = MakeShared<FNarrativeGraphNodeFactory>();
	FEdGraphUtilities::RegisterVisualNodeFactory(NodeFactory);
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		FName("CharacterStatInspector"),
		FOnSpawnTab::CreateRaw(this, &FSlateLearningEditorModule::SpawnStatInspectorTab))
		.SetDisplayName(INVTEXT("Character Stat Inspector"))
		.SetMenuType(ETabSpawnerMenuType::Enabled);
	
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools")
	.Get();
	
	NarrativeGraphActions = MakeShared<FNarrativeGraphAssetTypeActions>();
	AssetTools.RegisterAssetTypeActions(NarrativeGraphActions.ToSharedRef());
}

void FSlateLearningEditorModule::ShutdownModule()
{
	FNarrativeGraphEditorCommands::Unregister();
	
	if (NodeFactory.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(NodeFactory);
		NodeFactory.Reset();
	}
	
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(
		FName("CharacterStatInspector"));
	
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = 
			FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools")
			.Get();
		AssetTools.UnregisterAssetTypeActions(
			NarrativeGraphActions.ToSharedRef());
	}
	
}

TSharedRef<SDockTab> FSlateLearningEditorModule::SpawnStatInspectorTab(
	const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SStatInspectorPanel)
		];
}