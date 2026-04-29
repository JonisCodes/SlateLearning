// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "NarrativeGraphSchema.generated.h"

/**
 * 
 */
UCLASS()
class SLATELEARNINGEDITOR_API UNarrativeGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()
	
public:
	// Called when the user releases a wire onto empty space
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	
	// Called to determine if two pins can be connected
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	
	// Called when a connection is actually made
	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
	
	// The name shown in the graph title bar
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
	
	virtual bool ShouldAlwaysPurgeOnModification() const override { return false; }
	
	virtual bool IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const override
	{
		return false;
	}
	
	virtual bool CanDuplicateGraph(UEdGraph* InSourceGraph) const override
	{
		return true;
	}
	
};

USTRUCT()
struct FNarrativeSchemaAction_NewNode : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY()

	UClass* NodeClass;

	FNarrativeSchemaAction_NewNode() : NodeClass(nullptr) {}

	FNarrativeSchemaAction_NewNode(FText InNodeCategory, FText InMenuDesc,
		FText InToolTip, UClass* InNodeClass)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, 0)
		, NodeClass(InNodeClass)
	{}

	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
		const FVector2D Location, bool bSelectNewNode) override;
};
