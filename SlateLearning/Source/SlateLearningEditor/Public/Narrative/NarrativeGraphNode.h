// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
// #include "NarrativeGraphNode.h"
#include "NarrativeGraphNode.generated.h"

/**
 * 
 */

class UNarrativeGraphNode;
class UNarrativeGraphNode_Dialogue;
class UNarrativeGraphNode_Choice;

UCLASS(Abstract)
class SLATELEARNINGEDITOR_API UNarrativeGraphNode : public UEdGraphNode
{
	GENERATED_BODY()
	
public:
	UNarrativeGraphNode();
	
	// UEdGraphNode interface
	
	// The title shown at the top of the node
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	
	// The tooltip shown on hover
	virtual FText GetTooltipText() const override;
	
	// Called when the node is first placed on the graph
	virtual void AllocateDefaultPins() override;
	
	// Whether this node can be deleted by the user
	virtual bool CanDuplicateNode() const override { return true; }
	virtual bool CanUserDeleteNode() const override { return true; }
	
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	
	// Node color
	virtual FLinearColor GetNodeTitleColor() const override
	{
		return FLinearColor(0.1f, 0.1f, 0.1f);
	}
	
	// Helpers
	UEdGraphPin* GetOutputPin() const;
	UEdGraphPin* GetInputPin() const;
	
protected:
	// Pin name constants
	static const FName PinName_Input;
	static const FName PinName_Output;
};
