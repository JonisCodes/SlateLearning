#include "FNarrativeGraphAssetTypeActions.h"

#include "FNarrativeGraphEditor.h"
#include "NarrativeGraph.h"

UClass* FNarrativeGraphAssetTypeActions::GetSupportedClass() const
{
	return UNarrativeGraph::StaticClass();
}

void FNarrativeGraphAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	// FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() 
	? EToolkitMode::WorldCentric 
	: EToolkitMode::Standalone;
	
	for (const auto Object : InObjects)
	{
		const auto NarrativeGraph = Cast<UNarrativeGraph>(Object);
		if (!NarrativeGraph) continue;

		const auto Editor = MakeShared<FNarrativeGraphEditor>();
		
		Editor->InitNarrativeGraphEditor(Mode, EditWithinLevelEditor, NarrativeGraph);
	}
}
