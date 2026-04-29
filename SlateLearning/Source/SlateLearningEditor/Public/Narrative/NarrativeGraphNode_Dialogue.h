// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NarrativeGraphNode.h"
#include "NarrativeGraphNode_Dialogue.generated.h"

/**
 * 
 */
UCLASS()
class SLATELEARNINGEDITOR_API UNarrativeGraphNode_Dialogue : public UNarrativeGraphNode
{
	GENERATED_BODY()
	
public:
	// The actual dialogue text this node displays in game
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FText DialogueText;

	// Who is speaking this line
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FText SpeakerName;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override
	{
		if (TitleType == ENodeTitleType::FullTitle && !SpeakerName.IsEmpty())
		{
			return SpeakerName;
		}
		return INVTEXT("Dialogue");
	}

	virtual FText GetTooltipText() const override
	{
		return DialogueText.IsEmpty() 
			? INVTEXT("A dialogue line") 
			: DialogueText;
	}

	virtual FLinearColor GetNodeTitleColor() const override
	{
		return FLinearColor(0.1f, 0.3f, 0.6f); // blue
	}
};
