// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NarrativeGraph.h"
#include "NarrativeComponent.generated.h"

// Fired when a dialogue line should be displayed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueNodeReached,
											 const FText&, SpeakerName,
											 const FText&, DialogueText);

// Fired when the player needs to make a choice
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChoiceNodeReached,
											const TArray<FNarrativeNodeData>&, AvailableChoices);

// Fired when the graph has no more nodes to traverse
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNarrativeFinished);

// Fired when dialogue starts
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNarrativeStarted);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SLATELEARNING_API UNarrativeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UNarrativeComponent();
	
	// The graph asset to use, set in owning actor editor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Narrative")
	TObjectPtr<UNarrativeGraph> NarrativeGraph;
	
	// Events
	UPROPERTY(BlueprintAssignable, Category = "Narrative|Events")
	FOnNarrativeStarted OnNarrativeStarted;
	
	UPROPERTY(BlueprintAssignable, Category = "Narrative|Events")
	FOnDialogueNodeReached OnDialogueNodeReached;
	
	UPROPERTY(BlueprintAssignable, Category = "Narrative|Events")
	FOnChoiceNodeReached OnChoiceNodeReached;
	
	UPROPERTY(BlueprintAssignable, Category = "Narrative|Events")
	FOnNarrativeFinished OnNarrativeFinished;
	
	// public api
	
	// Start traversing the graph from the beginning
	UFUNCTION(BlueprintCallable, Category = "Narrative")
	void StartNarrative();
	
	// Advance past the current dialogue node
	UFUNCTION(BlueprintCallable, Category = "Narrative")
	void AdvanceDialogue();
	
	// Select a choice by index from the current choice node
	UFUNCTION(BlueprintCallable, Category = "Narrative")
	void SelectChoice(int32 ChoiceIndex);
	
	UFUNCTION(BlueprintPure, Category = "Narrative")
	bool IsNarrativeRunning() const { return bIsRunning; }
	
	UFUNCTION(BlueprintPure, Category = "Narrative")
	FNarrativeNodeData GetCurrentNode() const;
	
private:
	const FNarrativeNodeData* FindNode(const FGuid& NodeId) const;
	
	void ProcessCurrentNode();
	
	FGuid CurrentNodeId;
	
	bool bIsRunning = false;
	
	TArray<FGuid> CurrentChoices;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
