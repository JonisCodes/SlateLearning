// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryTypes.h"
#include "Widgets/SLeafWidget.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FOnSlotClicked, int32);
DECLARE_DELEGATE_OneParam(FOnSlotHovered, int32);
DECLARE_DELEGATE_OneParam(FOnSlotUnHovered, int32);
DECLARE_DELEGATE_TwoParams(FOnSlotDrop, int32, int32);


class SLATELEARNING_API SInventorySlot : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SInventorySlot)
			: _SlotData()
			  , _bSelected(false)
		{
		}

		SLATE_ARGUMENT(FInventorySlotData, SlotData)
		SLATE_ATTRIBUTE(bool, bSelected)
		SLATE_EVENT(FOnSlotClicked, OnSlotClicked)
		SLATE_EVENT(FOnSlotHovered, OnSlotHovered)
		SLATE_EVENT(FOnSlotUnHovered, OnSlotUnhovered)
		SLATE_EVENT(FOnSlotDrop, OnSlotDrop)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// SLeafWidget interface
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	                      const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
	                      int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	// Input
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

	// Drag and Drop
	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual bool ComputeVolatility() const override { return true; }

	void UpdateSlotData(const FInventorySlotData& NewData);

	bool IsSlotSet() const;

	FInventorySlotData GetSlotData() const;

private:
	FInventorySlotData SlotData;
	TAttribute<bool> bSelected;

	FOnSlotClicked OnSlotClickedDelegate;
	FOnSlotHovered OnSlotHoveredDelegate;
	FOnSlotUnHovered OnSlotUnHoveredDelegate;
	FOnSlotDrop OnSlotDropDelegate;

	bool bHovered = false;
};
