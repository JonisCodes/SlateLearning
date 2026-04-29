#pragma once

class UNarrativeGraph;

class FNarrativeGraphEditor : public FAssetEditorToolkit
{
public:
	FNarrativeGraphEditor();
	virtual ~FNarrativeGraphEditor();
	
	void InitNarrativeGraphEditor(const EToolkitMode::Type Mode,
		const TSharedPtr<IToolkitHost>& InitToolkitHost,
		UNarrativeGraph* InNarrativeGraph);
	
	// FAssetEditorToolkit interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	
	void OnFocusStart();
	
	// IToolkit interface
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	
	UNarrativeGraph* GetNarrativeGraph() const { return GetNarrativeGraph(); }
	
private:
	TSharedPtr<SGraphEditor> GraphEditorWidget;
	
	TSharedRef<SGraphEditor> CreateGraphEditorWidget();
	
	TSharedPtr<IDetailsView> DetailsView;
	
	void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection);
	void OnNodeDoubleClicked(UEdGraphNode* Node);
	void OnGraphChanged(const FEdGraphEditAction& Action);
	
	void CopySelectedNodes();
	void PasteNodes();
	void PasteNodesAtLocation(const FVector2D& PasteLocation);
	bool CanCopyNodes() const;
	bool CanPasteNodes() const;
	void CutSelectedNodes();
	bool CanCutNodes() const;

	// Currently selected nodes
	TSet<UObject*> SelectedNodes;
	
	// Tab spawner functions
	TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);
	
	void BuildToolbar();
	
	void BindCommands();
	
	void OnCompileGraph();
	void OnValidateGraph();
	void OnClearGraph();
	
	void DeleteSelectedNodes();
	bool CanDeleteSelectedNodes() const;
	
	UNarrativeGraph* NarrativeGraph = nullptr;
	
	TSharedPtr<FUICommandList> EditorCommandList;
	
	static const FName ViewportTabId;
	static const FName DetailsTabId;
};
