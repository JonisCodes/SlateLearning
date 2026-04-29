// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "NarrativeNodeData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FNarrativeNodeData
{
		GENERATED_BODY()
	
public:
		UPROPERTY(BlueprintReadOnly)
		FGuid NodeId;
	
		UPROPERTY(BlueprintReadOnly)
		FText SpeakerName;
	
		UPROPERTY(BlueprintReadOnly)
		FText DialogueText;
	
		UPROPERTY(BlueprintReadOnly)
		FText ChoiceText;
	
		UPROPERTY(BlueprintReadOnly)
		TArray<FGuid> ConnectedNodeIds;
	
		UPROPERTY(BlueprintReadOnly)
		FString NodeType; // "Start", "Dialogue", "Choice"
};
