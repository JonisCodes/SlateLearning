#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FNarrativeGraphNodeFactory;
class FNarrativeGraphAssetTypeActions;

class FSlateLearningEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedRef<SDockTab> SpawnStatInspectorTab(const FSpawnTabArgs& Args);
	TSharedPtr<FNarrativeGraphAssetTypeActions> NarrativeGraphActions;
	TSharedPtr<FNarrativeGraphNodeFactory> NodeFactory;
};
