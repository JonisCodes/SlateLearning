// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NarrativeGraphNode.h"
#include "NarrativeGraphNode_Choice.generated.h"

/**
 * 
 */
UCLASS()
class SLATELEARNINGEDITOR_API UNarrativeGraphNode_Choice : public UNarrativeGraphNode
{
	GENERATED_BODY()
	
public:
	// The text shown to the player as a choice option
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Choice")
	FText ChoiceText;

	// Optional condition that must be met for this choice to appear
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Choice")
	FText Condition;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override
	{
		return INVTEXT("Choice");
	}

	virtual FText GetTooltipText() const override
	{
		return ChoiceText.IsEmpty() 
			? INVTEXT("A player choice") 
			: ChoiceText;
	}

	virtual FLinearColor GetNodeTitleColor() const override
	{
		return FLinearColor(0.5f, 0.2f, 0.5f); // purple
	}

	virtual void AllocateDefaultPins() override
	{
		// Choice has one input but multiple outputs are possible
		// For now just one output - Phase 5 adds dynamic output pins
		CreatePin(EGPD_Input,  FName("exec"), PinName_Input);
		CreatePin(EGPD_Output, FName("exec"), PinName_Output);
	}
};
