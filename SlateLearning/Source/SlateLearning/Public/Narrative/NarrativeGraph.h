// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NarrativeNodeData.h"
#include "NarrativeGraph.generated.h"

/**
 * 
 */

class UNarrativeGraphSchema;

UCLASS()
class SLATELEARNING_API UNarrativeGraph : public UObject
{
	GENERATED_BODY()
	
public:
	UNarrativeGraph();
	
	// The graph name, editable in the details panel
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Narrative")
	FText GraphName;
	
	// Description of this narrative graph
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Narrative")
	FText Description;
	
	// Author for tracking purposes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Narrative")
	FString Author;
	
	UPROPERTY(BlueprintReadOnly, Category = "Narrative")
	TArray<FNarrativeNodeData> CompiledNodes;
	
	UPROPERTY(BlueprintReadOnly, Category = "Narrative")
	FGuid StartNodeId;
	
#if WITH_EDITOR
	void CompileGraph();
#endif
	
#if WITH_EDITOR
	TArray<FText> ValidateGraph() const;
#endif
	
	
#if WITH_EDITORONLY_DATA
	// The EdGraph that the editor works with - editor only, not shipped
	UPROPERTY()
	class UEdGraph* NarrativeEdGraph;
#endif
	
	void CreateGraph();
	
	// Called after the object is loaded from disk
	virtual void PostLoad() override;
	
	virtual void PostInitProperties() override;
	
	// Called before the object is saved to disk
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
	
};