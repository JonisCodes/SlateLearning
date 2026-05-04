// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryTypes.h"
#include "Widgets/SCompoundWidget.h"

class SItemInfoPanel;
class SDialogueWidget;
class SInventorySlot;

class SLATELEARNING_API SInventoryGrid : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInventoryGrid)
			: _Columns(5)
			  , _Rows(4)
		{
		}

		SLATE_ARGUMENT(int32, Columns)
		SLATE_ARGUMENT(int32, Rows)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void SetItemAtSlot(int32 SlotIndex, const FInventoryItem& Item);
	void ClearSlot(int32 SlotIndex);

private:
	void OnSlotClicked(int32 SlotIndex);
	void OnSlotHovered(int32 SlotIndex);
	void OnSlotUnhovered(int32 SlotIndex) const;
	void OnSlotDrop(const int32 DestinationIndex, const int32 SourceIndex);
	bool IsSlotSelected(int32 SlotIndex) const;
	void HandleKeyNavigation(const FKeyEvent& KeyEvent);

	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual bool SupportsKeyboardFocus() const override { return true; }

	TArray<FInventorySlotData> Slots;
	TArray<TSharedPtr<SInventorySlot>> SlotWidgets;
	int32 SelectedSlotIndex = INDEX_NONE;
	int32 Columns = 5;

	TSharedPtr<SItemInfoPanel> ItemInfoPanel;

	FVector2D ViewportSize;
	FVector2D InfoPanelPosition;
};
