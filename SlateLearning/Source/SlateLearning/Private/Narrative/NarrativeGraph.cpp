// Fill out your copyright notice in the Description page of Project Settings.


#include "NarrativeGraph.h"

#include "UObject/ObjectSaveContext.h"

#if WITH_EDITOR
#include "EdGraph/EdGraph.h"
#endif

class UNarrativeGraphNode_Start;

UNarrativeGraph::UNarrativeGraph()
{
	GraphName = INVTEXT("New Narrative Graph");
	Description = INVTEXT("");
	Author = TEXT("");

#if WITH_EDITORONLY_DATA
	NarrativeEdGraph = nullptr;
#endif
}

void UNarrativeGraph::PostInitProperties()
{
	Super::PostInitProperties();

#if WITH_EDITOR
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_NeedLoad))
	{
		CreateGraph();
	}
#endif
}

#if WITH_EDITOR
void UNarrativeGraph::CompileGraph()
{
	if (!NarrativeEdGraph) return;

	CompiledNodes.Empty();
	StartNodeId = FGuid();

	// First pass - create all node data with stable GUIDs
	// Use the EdGraphNode's existing GUID for stability across compiles
	TMap<FGuid, int32> NodeGuidToIndex;

	for (const UEdGraphNode* Node : NarrativeEdGraph->Nodes)
	{
		FNarrativeNodeData NodeData;
		NodeData.NodeId = Node->NodeGuid; // use existing guid for stability

		if (FString ClassName = Node->GetClass()->GetName(); ClassName.Contains(TEXT("Start")))
		{
			NodeData.NodeType = TEXT("Start");
			StartNodeId = NodeData.NodeId;
		}
		else if (ClassName.Contains(TEXT("Dialogue")))
		{
			NodeData.NodeType = TEXT("Dialogue");

			if (const FTextProperty* TextProp = FindFProperty<FTextProperty>(
				Node->GetClass(), "DialogueText"))
			{
				NodeData.DialogueText =
					TextProp->GetPropertyValue_InContainer(Node);
			}
			if (const FTextProperty* SpeakerProp = FindFProperty<FTextProperty>(
				Node->GetClass(), "SpeakerName"))
			{
				NodeData.SpeakerName =
					SpeakerProp->GetPropertyValue_InContainer(Node);
			}
		}
		else if (ClassName.Contains(TEXT("Choice")))
		{
			NodeData.NodeType = TEXT("Choice");

			if (FTextProperty* ChoiceProp = FindFProperty<FTextProperty>(
				Node->GetClass(), "ChoiceText"))
			{
				NodeData.ChoiceText =
					ChoiceProp->GetPropertyValue_InContainer(Node);
			}
		}

		int32 Index = CompiledNodes.Add(NodeData);
		NodeGuidToIndex.Add(NodeData.NodeId, Index);
	}

	// Second pass - wire up connections using output pins
	for (UEdGraphNode* Node : NarrativeEdGraph->Nodes)
	{
		const int32* SourceIndex = NodeGuidToIndex.Find(Node->NodeGuid);
		if (!SourceIndex) continue;

		for (UEdGraphPin* Pin : Node->Pins)
		{
			if (Pin->Direction != EGPD_Output) continue;

			for (const UEdGraphPin* LinkedPin : Pin->LinkedTo)
			{
				const UEdGraphNode* TargetNode = LinkedPin->GetOwningNode();

				UE_LOG(LogTemp, Warning, TEXT("Connection: %s -> %s | TargetGUID: %s"),
				       *Node->GetClass()->GetName(),
				       *TargetNode->GetClass()->GetName(),
				       *TargetNode->NodeGuid.ToString());

				if (const int32* TargetIndex = NodeGuidToIndex.Find(TargetNode->NodeGuid); !TargetIndex)
				{
					UE_LOG(LogTemp, Warning, TEXT("TARGET NOT FOUND IN MAP"));
					continue;
				}

				CompiledNodes[*SourceIndex].ConnectedNodeIds.Add(
					TargetNode->NodeGuid);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Compiled %d nodes, StartNodeId: %s"),
	       CompiledNodes.Num(), *StartNodeId.ToString());

	for (const FNarrativeNodeData& Node : CompiledNodes)
	{
		UE_LOG(LogTemp, Warning, TEXT("Compiled node: %s | ID: %s | Type: %s"),
		       *Node.DialogueText.ToString(),
		       *Node.NodeId.ToString(),
		       *Node.NodeType);
	}

	MarkPackageDirty();
}

TArray<FText> UNarrativeGraph::ValidateGraph() const
{
	TArray<FText> Errors;

	if (!NarrativeEdGraph)
	{
		Errors.Add(INVTEXT("Graph has not been initialized"));
		return Errors;
	}

	auto bHasStart = false;
	int32 DisconnectedNodes = 0;

	for (const auto Node : NarrativeEdGraph->Nodes)
	{
		FString ClassName = Node->GetClass()->GetName();

		if (ClassName.Contains("Start"))
		{
			bHasStart = true;

			// start node must have at least one connection
			auto bHasConnection = false;
			for (const auto Pin : Node->Pins)
			{
				if (Pin->Direction == EGPD_Output && Pin->LinkedTo.Num() > 0)
				{
					bHasConnection = true;
					break;
				}
			}

			if (!bHasConnection)
			{
				Errors.Add(INVTEXT("Start node has no connections"));
			}
		}

		// check for completely disconnected nodes
		auto bHasAnyConnection = false;
		for (const auto Pin : Node->Pins)
		{
			if (Pin->LinkedTo.Num() > 0)
			{
				bHasAnyConnection = true;
				break;
			}
		}

		if (!bHasAnyConnection && !ClassName.Contains("Start"))
		{
			DisconnectedNodes++;
		}
	}

	if (!bHasStart)
	{
		Errors.Add(INVTEXT("Graph has no Start node"));
	}

	if (DisconnectedNodes > 0)
	{
		Errors.Add(FText::Format(
			INVTEXT("{0} node(s) have no connections"),
			FText::AsNumber(DisconnectedNodes)));
	}

	if (GraphName.IsEmpty())
	{
		Errors.Add(INVTEXT("Graph has no name"));
	}

	return Errors;
}


void UNarrativeGraph::CreateGraph()
{
	if (NarrativeEdGraph) return;

	UE_LOG(LogTemp, Warning, TEXT("NarrativeGraph: Creating EdGraph"));

	NarrativeEdGraph = NewObject<UEdGraph>(
		this,
		UEdGraph::StaticClass(),
		NAME_None,
		RF_Transactional);

	const auto SchemaClass = LoadObject<UClass>(nullptr,
	                                            TEXT("/Script/SlateLearningEditor.NarrativeGraphSchema"));

	if (SchemaClass)
	{
		NarrativeEdGraph->Schema = SchemaClass;
	}

	NarrativeEdGraph->bAllowDeletion = false;
}
#endif


void UNarrativeGraph::PostLoad()
{
	Super::PostLoad();
	UE_LOG(LogTemp, Warning, TEXT("NarrativeGraph loaded: %s"),
	       *GraphName.ToString());
}

void UNarrativeGraph::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
}
