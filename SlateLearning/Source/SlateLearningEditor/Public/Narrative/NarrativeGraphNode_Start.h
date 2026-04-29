// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NarrativeGraphNode.h"
#include "NarrativeGraphNode_Start.generated.h"

/**
 * 
 */
UCLASS()
class SLATELEARNINGEDITOR_API UNarrativeGraphNode_Start : public UNarrativeGraphNode
{
	GENERATED_BODY()
	
public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override
	{
		return INVTEXT("Start");
	}
	
	virtual FText GetTooltipText() const override
	{
		return INVTEXT("The entry point of the narrative graph");
	}
	
	virtual bool CanUserDeleteNode() const override
	{
		return false; // start node can never be deleted
	}
	
	virtual bool CanDuplicateNode() const override { return false; }

	virtual FLinearColor GetNodeTitleColor() const override
	{
		return FLinearColor(0.0f, 0.5f, 0.0f); // green
	}

	virtual void AllocateDefaultPins() override
	{
		// Start node only has an output - no input
		CreatePin(EGPD_Output, FName("exec"), PinName_Output);
	}
};
