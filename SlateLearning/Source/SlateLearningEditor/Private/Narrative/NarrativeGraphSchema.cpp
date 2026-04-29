// Fill out your copyright notice in the Description page of Project Settings.


#include "NarrativeGraphSchema.h"

#include "NarrativeGraphNode.h"
#include "NarrativeGraphNode_Choice.h"
#include "NarrativeGraphNode_Dialogue.h"

#define LOCTEXT_NAMESPACE "NarrativeGraphSchema"

void UNarrativeGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FNarrativeSchemaAction_NewNode> DialogueAction = 
		MakeShared<FNarrativeSchemaAction_NewNode>(
			LOCTEXT("NarrativeNodes", "Narrative"),
			LOCTEXT("AddDialogue", "Add Dialogue Node"),
			LOCTEXT("AddDialogueTip", "Adds a dialogue line node"),
			UNarrativeGraphNode_Dialogue::StaticClass());
	
	ContextMenuBuilder.AddAction(DialogueAction);
	
	TSharedPtr<FNarrativeSchemaAction_NewNode> ChoiceAction = 
		MakeShared<FNarrativeSchemaAction_NewNode>(
			LOCTEXT("NarrativeNodes", "Narrative"),
			LOCTEXT("AddChoice", "Add Choice Node"),
			LOCTEXT("AddChoiceTip", "Adds a player choice node"),
			UNarrativeGraphNode_Choice::StaticClass());
	
	ContextMenuBuilder.AddAction(ChoiceAction);
}

const FPinConnectionResponse UNarrativeGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW,
			LOCTEXT("NoSelfConnection", "Cannot connect a node to itself"));
	}
	
	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW,
			LOCTEXT("NoPinDirectionMatch", "Cannot connect pins of the same directions"));
	}
	
	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE,
		LOCTEXT("ConnectionAllowed", "Connect nodes"));
}

bool UNarrativeGraphSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	return UEdGraphSchema::TryCreateConnection(A, B);
}

FLinearColor UNarrativeGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor::White;
}

UEdGraphNode* FNarrativeSchemaAction_NewNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
		const FVector2D Location, bool bSelectNewNode)
{
	if (!NodeClass) return nullptr;

	ParentGraph->Modify();

	UNarrativeGraphNode* NewNode = NewObject<UNarrativeGraphNode>(
		ParentGraph, NodeClass);

	NewNode->NodePosX = Location.X;
	NewNode->NodePosY = Location.Y;
	NewNode->CreateNewGuid();
	NewNode->PostPlacedNewNode();
	NewNode->AllocateDefaultPins();

	ParentGraph->AddNode(NewNode, true, bSelectNewNode);

	if (FromPin)
	{
		NewNode->AutowireNewNode(FromPin);
	}

	return NewNode;
}

#undef LOCTEXT_NAMESPACE
