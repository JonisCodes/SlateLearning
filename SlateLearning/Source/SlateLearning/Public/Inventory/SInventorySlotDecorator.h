// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryTypes.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SLATELEARNING_API SInventorySlotDecorator : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInventorySlotDecorator)
			: _SlotItem()
			  , _Slot()
			  , _SlotSize()
		{
		}

		SLATE_ATTRIBUTE(FInventoryItem, SlotItem);
		SLATE_ATTRIBUTE(int32, Slot);
		SLATE_ATTRIBUTE(FVector2D, SlotSize)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
