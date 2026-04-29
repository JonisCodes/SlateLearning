// Fill out your copyright notice in the Description page of Project Settings.


#include "NarrativeComponent.h"
#include "NarrativeGraph.h"


// Sets default values for this component's properties
UNarrativeComponent::UNarrativeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UNarrativeComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UNarrativeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UNarrativeComponent::StartNarrative()
{
	UE_LOG(LogTemp, Warning, TEXT("=== Runtime CompiledNodes ==="));
	for (const FNarrativeNodeData& Node : NarrativeGraph->CompiledNodes)
	{
		UE_LOG(LogTemp, Warning, TEXT("Node: %s | ID: %s | Connections: %d"),
			*Node.NodeType, *Node.NodeId.ToString(), 
			Node.ConnectedNodeIds.Num());
        
		for (const FGuid& ConnId : Node.ConnectedNodeIds)
		{
			UE_LOG(LogTemp, Warning, TEXT("  -> %s"), *ConnId.ToString());
		}
	}
	
	if (!NarrativeGraph)
	{
		UE_LOG(LogTemp, Warning, TEXT("NarrativeComponent: No graph assigned"));
		return;
	}

	if (NarrativeGraph->CompiledNodes.Num() == 0)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("NarrativeComponent: Graph has not been compiled"));
		return;
	}

	if (!NarrativeGraph->StartNodeId.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("NarrativeComponent: No valid start node"));
		return;
	}

	bIsRunning = true;
	CurrentNodeId = NarrativeGraph->StartNodeId;

	OnNarrativeStarted.Broadcast();

	// Start node just leads to the first real node
	AdvanceDialogue();
}

void UNarrativeComponent::AdvanceDialogue()
{
	if (!bIsRunning) return;

	const FNarrativeNodeData* CurrentNode = FindNode(CurrentNodeId);
	if (!CurrentNode)
	{
		bIsRunning = false;
		OnNarrativeFinished.Broadcast();
		return;
	}

	if (CurrentNode->ConnectedNodeIds.Num() == 0)
	{
		bIsRunning = false;
		OnNarrativeFinished.Broadcast();
		return;
	}

	// Check if any connected nodes are Choice nodes
	TArray<FGuid> ChoiceGuids;
	for (const FGuid& ConnectedId : CurrentNode->ConnectedNodeIds)
	{
		const FNarrativeNodeData* ConnectedNode = FindNode(ConnectedId);
		if (ConnectedNode && ConnectedNode->NodeType == TEXT("Choice"))
		{
			ChoiceGuids.Add(ConnectedId);
		}
	}

	if (ChoiceGuids.Num() > 0)
	{
		// Store GUIDs and build display data separately
		CurrentChoices = ChoiceGuids;

		TArray<FNarrativeNodeData> ChoiceData;
		for (const FGuid& ChoiceId : CurrentChoices)
		{
			const FNarrativeNodeData* Node = FindNode(ChoiceId);
			if (Node)
			{
				ChoiceData.Add(*Node);
			}
		}

		OnChoiceNodeReached.Broadcast(ChoiceData);
	}
	else
	{
		// No choices - advance to next dialogue node directly
		CurrentNodeId = CurrentNode->ConnectedNodeIds[0];
		ProcessCurrentNode();
	}
}

void UNarrativeComponent::SelectChoice(int32 ChoiceIndex)
{
	if (!bIsRunning) return;

	if (!CurrentChoices.IsValidIndex(ChoiceIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid choice index %d"), ChoiceIndex);
		return;
	}

	// Look up fresh from CompiledNodes using the stored GUID
	const FNarrativeNodeData* ChoiceNode = FindNode(CurrentChoices[ChoiceIndex]);
	CurrentChoices.Empty();

	if (!ChoiceNode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Choice node not found"));
		bIsRunning = false;
		OnNarrativeFinished.Broadcast();
		return;
	}

	if (ChoiceNode->ConnectedNodeIds.Num() == 0)
	{
		bIsRunning = false;
		OnNarrativeFinished.Broadcast();
		return;
	}

	CurrentNodeId = ChoiceNode->ConnectedNodeIds[0];
	ProcessCurrentNode();
}

void UNarrativeComponent::ProcessCurrentNode()
{
	const FNarrativeNodeData* Node = FindNode(CurrentNodeId);
	if (!Node)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProcessCurrentNode: Node NOT FOUND for ID %s"), 
			*CurrentNodeId.ToString());
		bIsRunning = false;
		OnNarrativeFinished.Broadcast();
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("ProcessCurrentNode: Found node type '%s', text '%s'"), 
		*Node->NodeType, *Node->DialogueText.ToString());

	if (Node->NodeType == TEXT("Dialogue"))
	{
		OnDialogueNodeReached.Broadcast(Node->SpeakerName, Node->DialogueText);
	}
	else if (Node->NodeType == TEXT("Start"))
	{
		AdvanceDialogue();
	}
}

const FNarrativeNodeData* UNarrativeComponent::FindNode(const FGuid& NodeId) const
{
	if (!NarrativeGraph) return nullptr;
	
	for (const FNarrativeNodeData& Node : NarrativeGraph->CompiledNodes)
	{
		if (Node.NodeId == NodeId) return &Node;
	}
	return nullptr;
}

FNarrativeNodeData UNarrativeComponent::GetCurrentNode() const
{
	const auto Node = FindNode(CurrentNodeId);
	return Node ? *Node : FNarrativeNodeData();
}


