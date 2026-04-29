#include "FNarrativeGraphEditor.h"

#include "EdGraphUtilities.h"
#include "FNarrativeGraphEditorCommands.h"
#include "NarrativeGraph.h"
#include "Misc/MessageDialog.h"
#include "NarrativeGraphNode_Start.h"

#include "PropertyEditorClipboard.h"
#include "HAL/PlatformApplicationMisc.h"

#include "GenericPlatform/GenericPlatformMisc.h"
#include "Framework/Commands/GenericCommands.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

const FName FNarrativeGraphEditor::ViewportTabId = FName("NarrativeGraphEditor_Viewport");
const FName FNarrativeGraphEditor::DetailsTabId = FName("NarrativeGraphEditor_Details");

FNarrativeGraphEditor::FNarrativeGraphEditor()
{
}

FNarrativeGraphEditor::~FNarrativeGraphEditor()
{
}

void FNarrativeGraphEditor::InitNarrativeGraphEditor(const EToolkitMode::Type Mode,
                                                     const TSharedPtr<IToolkitHost>& InitToolkitHost,
                                                     UNarrativeGraph* InNarrativeGraph)
{
	NarrativeGraph = InNarrativeGraph;
	
	if (!NarrativeGraph->NarrativeEdGraph)
	{
		NarrativeGraph->CreateGraph();
	}
	
	if (NarrativeGraph->NarrativeEdGraph->Nodes.Num() == 0)
	{
		FGraphNodeCreator<UNarrativeGraphNode_Start> NodeCreator(
			*NarrativeGraph->NarrativeEdGraph);
		UNarrativeGraphNode_Start* StartNode = NodeCreator.CreateNode();
		StartNode->NodePosX = 0;
		StartNode->NodePosY = 0;
		NodeCreator.Finalize();
	}
	
	if (NarrativeGraph->NarrativeEdGraph)
	{
		UE_LOG(LogTemp, Warning, TEXT("Graph has %d nodes"), 
			NarrativeGraph->NarrativeEdGraph->Nodes.Num());
	}

	// Make sure the asset wont be garbage collected while editing it
	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->NotifyAssetOpened(NarrativeGraph, this);

	BindCommands();

	const TSharedRef<FTabManager::FLayout> DefaultLayout =
		FTabManager::NewLayout("NarrativeGraphEditor_Layout_v1")->AddArea(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Horizontal)
			                             ->Split
			                             (
				                             FTabManager::NewStack()
				                             ->SetSizeCoefficient(0.75f)
				                             ->AddTab(ViewportTabId, ETabState::OpenedTab)
			                             )

			                             ->Split
			                             (
				                             FTabManager::NewStack()
				                             ->SetSizeCoefficient(0.25f)
				                             ->AddTab(DetailsTabId, ETabState::OpenedTab)
			                             )
		);

	InitAssetEditor(Mode, InitToolkitHost, FName("NarrativeGraphEditor"), DefaultLayout, true, true, InNarrativeGraph);

	BuildToolbar();
}

FName FNarrativeGraphEditor::GetToolkitFName() const
{
	return FName("NarrativeGraphEditor");
}

FText FNarrativeGraphEditor::GetBaseToolkitName() const
{
	return INVTEXT("Narrative Graph Editor");
}

FString FNarrativeGraphEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("NarrativeGraph ");
}

FLinearColor FNarrativeGraphEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.7f, 0.2f, 0.2f);
}

void FNarrativeGraphEditor::OnFocusStart()
{
	if (!GraphEditorWidget.IsValid()) return;

	for (const auto Node : NarrativeGraph->NarrativeEdGraph->Nodes)
	{
		if (Node->GetClass()->GetName().Contains("Start"))
		{
			GraphEditorWidget->JumpToNode(Node, false);
			return;
		}
	}
}

void FNarrativeGraphEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	// Viewport tab
	InTabManager->RegisterTabSpawner(ViewportTabId,
	                                 FOnSpawnTab::CreateSP(this, &FNarrativeGraphEditor::SpawnTab_Viewport))
	            .SetDisplayName(INVTEXT("Viewport"))
	            .SetGroup(WorkspaceMenuCategory.ToSharedRef());
	
	// details tab
	InTabManager->RegisterTabSpawner(DetailsTabId,
		FOnSpawnTab::CreateSP(this, &FNarrativeGraphEditor::SpawnTab_Details))
	.SetDisplayName(INVTEXT("Details"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FNarrativeGraphEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	
	InTabManager->UnregisterTabSpawner(ViewportTabId);
	InTabManager->UnregisterTabSpawner(DetailsTabId);
}

TSharedRef<SGraphEditor> FNarrativeGraphEditor::CreateGraphEditorWidget()
{
	SGraphEditor::FGraphEditorEvents GraphEvents;
	
	GraphEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(
		this, &FNarrativeGraphEditor::OnSelectedNodesChanged);
	
	GraphEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateSP(
		this, &FNarrativeGraphEditor::OnNodeDoubleClicked);
	
	// visual appearance of the graph
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = INVTEXT("NARRATIVE GRAPH");
	AppearanceInfo.InstructionText = INVTEXT("Right-click to add nodes");
	
	// Create the widget
	TSharedRef<SGraphEditor> NewGraphEditor = SNew(SGraphEditor)
		.AdditionalCommands(EditorCommandList)
		.Appearance(AppearanceInfo)
		.GraphToEdit(NarrativeGraph->NarrativeEdGraph)
		.GraphEvents(GraphEvents)
		.ShowGraphStateOverlay(false);
	
	NarrativeGraph->NarrativeEdGraph->AddOnGraphChangedHandler(
	FOnGraphChanged::FDelegate::CreateLambda([this](const FEdGraphEditAction& Action)
	{
		OnGraphChanged(Action);
	}));
	
	return NewGraphEditor;
}

void FNarrativeGraphEditor::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{
	SelectedNodes = NewSelection;
	
	if (!DetailsView.IsValid()) return;
	
	if (NewSelection.Num() == 0)
	{
		// nothing selected
		DetailsView->SetObject(NarrativeGraph);
	}
	else if (NewSelection.Num() == 1)
	{
		// Single node selected, show its properties
		const auto SelectedNode = *NewSelection.CreateConstIterator();
		DetailsView->SetObject(SelectedNode);
	}
	else
	{
		// Multiple node selected, show all
		const auto SelectedArray = NewSelection.Array();
		DetailsView->SetObjects(SelectedArray);
	}
	
	// for (const auto Node : SelectedNodes)
	// {
	// 	if (UEdGraphNode* GraphNode = Cast<UEdGraphNode>(Node))
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Selected: %s"), 
	// 			*GraphNode->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
	// 	}
	// }
}

void FNarrativeGraphEditor::OnNodeDoubleClicked(UEdGraphNode* Node)
{
	UE_LOG(LogTemp, Warning, TEXT("Double clicked: %s"),
		*Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
}

void FNarrativeGraphEditor::OnGraphChanged(const FEdGraphEditAction& Action)
{
	if (NarrativeGraph)
	{
		NarrativeGraph->MarkPackageDirty();
	}
}

void FNarrativeGraphEditor::CopySelectedNodes()
{
	FGraphPanelSelectionSet NodesToCopy = GraphEditorWidget->GetSelectedNodes();
    
	UE_LOG(LogTemp, Warning, TEXT("Trying to copy %d nodes"), NodesToCopy.Num());
	
	
	TArray<UObject*> NodesToRemove;
	
	for (const auto Node : NodesToCopy)
	{
		const auto GraphNode = Cast<UEdGraphNode>(Node);
		if (GraphNode && !GraphNode->CanDuplicateNode())
		{
			NodesToRemove.Add(GraphNode);
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Node: %s CanDuplicate: %s"), 
		*GraphNode->GetClass()->GetName(),
		GraphNode->CanDuplicateNode() ? TEXT("YES") : TEXT("NO"));
	}
	
	for (const auto Node : NodesToRemove)
	{
			NodesToCopy.Remove(Node);
	}
    
	for (UObject* Node : NodesToCopy)
	{
		if (UEdGraphNode* GraphNode = Cast<UEdGraphNode>(Node))
			GraphNode->PrepareForCopying();
	}

	FString ExportedText;
	FEdGraphUtilities::ExportNodesToText(NodesToCopy, ExportedText);
    
	UE_LOG(LogTemp, Warning, TEXT("Exported text length: %d"), ExportedText.Len());
	UE_LOG(LogTemp, Warning, TEXT("Exported: %s"), *ExportedText);
    
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

bool FNarrativeGraphEditor::CanCopyNodes() const
{
	return SelectedNodes.Num() > 0;
}

void FNarrativeGraphEditor::PasteNodes()
{
	FVector2f PasteLocationF = GraphEditorWidget->GetPasteLocation2f();
	PasteNodesAtLocation(FVector2D(PasteLocationF.X, PasteLocationF.Y));
}

void FNarrativeGraphEditor::PasteNodesAtLocation(const FVector2D& PasteLocation)
{
	FString TextToImport;
	FPlatformApplicationMisc::ClipboardPaste(TextToImport);
    
	UE_LOG(LogTemp, Warning, TEXT("Paste text length: %d"), TextToImport.Len());
    
	bool bCanImport = FEdGraphUtilities::CanImportNodesFromText(
		NarrativeGraph->NarrativeEdGraph, TextToImport);
    
	UE_LOG(LogTemp, Warning, TEXT("Can import: %s"), bCanImport ? TEXT("YES") : TEXT("NO"));

	if (!bCanImport) return;

	const FScopedTransaction Transaction(INVTEXT("Paste Nodes"));
	UEdGraph* Graph = NarrativeGraph->NarrativeEdGraph;
	Graph->Modify();
	GraphEditorWidget->ClearSelectionSet();

	TSet<UEdGraphNode*> PastedNodes;
	FEdGraphUtilities::ImportNodesFromText(Graph, TextToImport, PastedNodes);

	UE_LOG(LogTemp, Warning, TEXT("Pasted %d nodes"), PastedNodes.Num());
	
	FPlatformApplicationMisc::ClipboardPaste(TextToImport);

	FVector2D AvgNodePosition(0.f, 0.f);
	for (UEdGraphNode* Node : PastedNodes)
	{
		AvgNodePosition.X += Node->NodePosX;
		AvgNodePosition.Y += Node->NodePosY;
	}
	if (PastedNodes.Num() > 0)
		AvgNodePosition /= static_cast<float>(PastedNodes.Num());

	for (UEdGraphNode* Node : PastedNodes)
	{
		Node->NodePosX = Node->NodePosX - AvgNodePosition.X + PasteLocation.X;
		Node->NodePosY = Node->NodePosY - AvgNodePosition.Y + PasteLocation.Y;
		Node->SnapToGrid(16.f);
		GraphEditorWidget->SetNodeSelection(Node, true);
	}

	GraphEditorWidget->NotifyGraphChanged();
	NarrativeGraph->MarkPackageDirty();
}

bool FNarrativeGraphEditor::CanPasteNodes() const
{
	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	return FEdGraphUtilities::CanImportNodesFromText(
		NarrativeGraph->NarrativeEdGraph, ClipboardContent);
}

void FNarrativeGraphEditor::CutSelectedNodes()
{
	CopySelectedNodes();
	DeleteSelectedNodes();
}

bool FNarrativeGraphEditor::CanCutNodes() const
{
	return CanCopyNodes() && CanDeleteSelectedNodes();
}

TSharedRef<SDockTab> FNarrativeGraphEditor::SpawnTab_Viewport(const FSpawnTabArgs& Args)
{
	GraphEditorWidget = CreateGraphEditorWidget();
	return SNew(SDockTab)
		.TabRole(PanelTab)
		[
			GraphEditorWidget.ToSharedRef()
		];
}

TSharedRef<SDockTab> FNarrativeGraphEditor::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	FPropertyEditorModule& PropertyModule =
	FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsArgs;
	DetailsArgs.bHideSelectionTip = true;
	DetailsArgs.bAllowSearch = true;

	DetailsView = PropertyModule.CreateDetailView(DetailsArgs);
	DetailsView->SetObject(NarrativeGraph);
	DetailsView->OnFinishedChangingProperties().AddLambda([this](const FPropertyChangedEvent& Event)
	{
		if (GraphEditorWidget.IsValid())
		{
			GraphEditorWidget->NotifyGraphChanged();
		}
	});

	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			DetailsView.ToSharedRef()
		];
}

void FNarrativeGraphEditor::BuildToolbar()
{
	TSharedPtr<FExtender> ToolbarExtender = MakeShared<FExtender>();
    
	ToolbarExtender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		EditorCommandList,
		FToolBarExtensionDelegate::CreateLambda(
			[](FToolBarBuilder& ToolbarBuilder)
			{
				ToolbarBuilder.BeginSection("Narrative");
                
				ToolbarBuilder.AddToolBarButton(
					FNarrativeGraphEditorCommands::Get().CompileGraph);
                
				ToolbarBuilder.AddSeparator();
                
				ToolbarBuilder.AddToolBarButton(
					FNarrativeGraphEditorCommands::Get().ValidateGraph);

				ToolbarBuilder.AddToolBarButton(
					FNarrativeGraphEditorCommands::Get().ClearGraph);
				
				ToolbarBuilder.AddToolBarButton(
					FNarrativeGraphEditorCommands::Get().FocusStart);
                
				ToolbarBuilder.EndSection();
			}));

	AddToolbarExtender(ToolbarExtender);
	RegenerateMenusAndToolbars();
}

void FNarrativeGraphEditor::BindCommands()
{
	EditorCommandList = MakeShared<FUICommandList>();
	
	const FNarrativeGraphEditorCommands& Commands = FNarrativeGraphEditorCommands::Get();
	
	EditorCommandList->MapAction(Commands.CompileGraph,
		FExecuteAction::CreateSP(this, &FNarrativeGraphEditor::OnCompileGraph));
	
	EditorCommandList->MapAction(Commands.ValidateGraph,
	FExecuteAction::CreateSP(this, &FNarrativeGraphEditor::OnValidateGraph));
	
	EditorCommandList->MapAction(Commands.ClearGraph,
	FExecuteAction::CreateSP(this, &FNarrativeGraphEditor::OnClearGraph));
	
	EditorCommandList->MapAction(FGenericCommands::Get().Delete,
		FExecuteAction::CreateSP(this, &FNarrativeGraphEditor::DeleteSelectedNodes),
		FCanExecuteAction::CreateSP(this, &FNarrativeGraphEditor::CanDeleteSelectedNodes));
	
	// Focus Start node
	EditorCommandList->MapAction(Commands.FocusStart,
		FExecuteAction::CreateSP(this, &FNarrativeGraphEditor::OnFocusStart));
	
	EditorCommandList->MapAction(FGenericCommands::Get().Copy,
	FExecuteAction::CreateSP(this, &FNarrativeGraphEditor::CopySelectedNodes),
	FCanExecuteAction::CreateSP(this, &FNarrativeGraphEditor::CanCopyNodes));

	EditorCommandList->MapAction(FGenericCommands::Get().Paste,
		FExecuteAction::CreateSP(this, &FNarrativeGraphEditor::PasteNodes),
		FCanExecuteAction::CreateSP(this, &FNarrativeGraphEditor::CanPasteNodes));

	EditorCommandList->MapAction(FGenericCommands::Get().Cut,
		FExecuteAction::CreateSP(this, &FNarrativeGraphEditor::CutSelectedNodes),
		FCanExecuteAction::CreateSP(this, &FNarrativeGraphEditor::CanCutNodes));
}

void FNarrativeGraphEditor::OnCompileGraph()
{
	if (!NarrativeGraph) return;
	
	NarrativeGraph->CompileGraph();
	
	// Show notification
	FNotificationInfo Info(FText::Format(
		INVTEXT("Compiled '{0}' - {1} nodes"),
		FText::FromString(NarrativeGraph->GetName()),
		FText::AsNumber(NarrativeGraph->CompiledNodes.Num())));
	
	Info.bFireAndForget = true;
	Info.ExpireDuration = 3.f;
	Info.bUseSuccessFailIcons = true;
	
	FSlateNotificationManager::Get().AddNotification(Info);
	
	UE_LOG(LogTemp, Warning, TEXT("Compile: %s"), 
		*NarrativeGraph->GraphName.ToString());
}

void FNarrativeGraphEditor::OnValidateGraph()
{
	if (!NarrativeGraph) return;
	
	const auto Errors = NarrativeGraph->ValidateGraph();
	
	if (Errors.Num() == 0)
	{
		FNotificationInfo Info(INVTEXT("Graph is valid"));
		Info.bFireAndForget = true;
		Info.ExpireDuration = 2.f;
		Info.bUseSuccessFailIcons = true;
		Info.Image =  FAppStyle::GetBrush("Icons.Success");
		FSlateNotificationManager::Get().AddNotification(Info);
	}
	else
	{
		FString ErrorMessage;
		for (const auto Error : Errors)
		{
			ErrorMessage += TEXT("• ") + Error.ToString() + TEXT("\n");
		}
		
		FNotificationInfo Info(FText::Format(
			INVTEXT("Validation failed:\n{0}"),
			FText::FromString(ErrorMessage)));
		Info.bFireAndForget = true;
		Info.ExpireDuration = 6.f;
		Info.bUseSuccessFailIcons = true;
		Info.Image =  FAppStyle::GetBrush("Icons.Error");
		FSlateNotificationManager::Get().AddNotification(Info);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Validate: %s"), 
	   *NarrativeGraph->GraphName.ToString());
}

void FNarrativeGraphEditor::OnClearGraph()
{
	if (!NarrativeGraph || !NarrativeGraph->NarrativeEdGraph) return;
	
	// Show confirmation dialog
	const auto Result = FMessageDialog::Open(
		EAppMsgType::YesNo,
		INVTEXT("Are you sure you want to remove all nodes except the Start node?"));
	
	if (Result != EAppReturnType::Yes) return;
	
	const FScopedTransaction Transaction(INVTEXT("Clear Graph"));
	NarrativeGraph->NarrativeEdGraph->Modify();
	
	// Collect nodes to remove
	TArray<UEdGraphNode*> NodesToRemove;
	for (const auto Node : NarrativeGraph->NarrativeEdGraph->Nodes)
	{
		if (Node->CanUserDeleteNode())
		{
			NodesToRemove.Add(Node);
		}
	}
	
	for (const auto Node : NodesToRemove)
	{
		NarrativeGraph->NarrativeEdGraph->RemoveNode(Node);
	}
	
	if (GraphEditorWidget.IsValid())
	{
		GraphEditorWidget->NotifyGraphChanged();
	}
	
	NarrativeGraph->MarkPackageDirty();
	
	UE_LOG(LogTemp, Warning, TEXT("Clear: %s"), 
		*NarrativeGraph->GraphName.ToString());
}

void FNarrativeGraphEditor::DeleteSelectedNodes()
{
	if (!GraphEditorWidget.IsValid()) return;
	
	const FScopedTransaction Transaction(INVTEXT("Delete Nodes"));
	NarrativeGraph->NarrativeEdGraph->Modify();
	
	for (const auto NodeObj : SelectedNodes)
	{
		if (const auto Node = Cast<UEdGraphNode>(NodeObj); Node && Node->CanUserDeleteNode())
		{
			GraphEditorWidget->GetCurrentGraph()->RemoveNode(Node);
		}
	}
}

bool FNarrativeGraphEditor::CanDeleteSelectedNodes() const
{
	if (SelectedNodes.IsEmpty()) return false;
	
	for (const auto NodeObj : SelectedNodes)
	{
		if (const auto Node = Cast<UEdGraphNode>(NodeObj); Node && Node->CanUserDeleteNode())
		{
			return true;
		}
	}
	return false; 
}
