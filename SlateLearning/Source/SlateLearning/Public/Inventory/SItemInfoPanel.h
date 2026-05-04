// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */

class SLATELEARNING_API SItemInfoPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SItemInfoPanel)
		// : _ItemName()
		// , _ItemDescription()
	{}
		// SLATE_ATTRIBUTE(FText, ItemName);
		// SLATE_ATTRIBUTE(FText, ItemDescription);
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
	void SetTooltip(const FText& InName, const FText& InDescription);
	
private:
	FText Name = FText::FromString("Item Name");
	FText Description = FText::FromString("Item Description");
};
