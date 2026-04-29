// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class UNarrativeGraphNode;
/**
 * 
 */
class SLATELEARNINGEDITOR_API SNarrativeGraphNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SNarrativeGraphNode)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, UEdGraphNode* InNode);
	
	// SGraphNode interface
	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	virtual bool IsNameReadOnly() const override { return true; }
	
protected:
	virtual TSharedRef<SWidget> BuildNodeContent();
	
	UNarrativeGraphNode* GetNarrativeNode() const;
	
	TSharedPtr<SVerticalBox> LeftNodeBox; // input
	TSharedPtr<SVerticalBox> RightNodeBox; // output
};
