// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogueWidgetBridge.generated.h"

struct FNarrativeNodeData;
class SDialogueWidget;
/**
 * 
 */
UCLASS()
class SLATELEARNING_API UDialogueWidgetBridge : public UObject
{
	GENERATED_BODY()
	
public:
	TWeakPtr<SDialogueWidget> SlateWidget;
	
	UFUNCTION()
	void OnDialogueNodeReached(const FText& SpeakerName, const FText& DialogueText);
	
	UFUNCTION()
	void OnChoiceNodeReached(const TArray<FNarrativeNodeData>& Choices);
	
	UFUNCTION()
	void OnNarrativeFinished();
};
