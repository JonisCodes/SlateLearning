// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNarrativeGraphNode.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SLATELEARNINGEDITOR_API SNarrativeGraphNode_Dialogue : public SNarrativeGraphNode
{
public:
	SLATE_BEGIN_ARGS(SNarrativeGraphNode_Dialogue)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, UEdGraphNode* InNode);
	
protected:
	virtual TSharedRef<SWidget> BuildNodeContent() override;
};
