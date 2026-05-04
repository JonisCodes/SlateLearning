// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SInventoryGrid.h"

#include "FunctionalUIScreenshotTest.h"
#include "SlateOptMacros.h"
#include "Components/Button.h"
#include "Inventory/SInventorySlot.h"
#include "Inventory/SItemInfoPanel.h"
#include "Widgets/SCanvas.h"
#include "Widgets/Layout/SUniformGridPanel.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SInventoryGrid::Construct(const FArguments& InArgs)
{
	Columns = InArgs._Columns;
	const int32 Rows = InArgs._Rows;
	const int32 TotalSlots = Columns * Rows;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	Slots.SetNum(TotalSlots);
	for (int32 i = 0; i < TotalSlots; i++)
	{
		Slots[i].SlotIndex = i;
	}

	TSharedPtr<SUniformGridPanel> Grid;
	ChildSlot
	[
		SNew(SCanvas)

		+ SCanvas::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Right)
		.Position(ViewportSize / 2)
		.Size(FVector2D(800.f, 800.f))
		[
			SAssignNew(Grid, SUniformGridPanel)
			.SlotPadding(FMargin(4.f))
		]

		+ SCanvas::Slot()
		.Position(TAttribute<FVector2D>::CreateLambda([this]()
		{
			return InfoPanelPosition;
		}))
		.Size(FVector2D(150.f, 200.f))
		[
			SAssignNew(ItemInfoPanel, SItemInfoPanel)
		]
	];

	for (int32 i = 0; i < TotalSlots; ++i)
	{
		const int32 Col = i % Columns;
		const int32 Row = i / Columns;

		TSharedPtr<SInventorySlot> SlotWidget;
		SAssignNew(SlotWidget, SInventorySlot)
		.SlotData(Slots[i])
		.bSelected(this, &SInventoryGrid::IsSlotSelected, i)
		.OnSlotClicked(this, &SInventoryGrid::OnSlotClicked)
		.OnSlotHovered(this, &SInventoryGrid::OnSlotHovered)
		.OnSlotUnhovered(this, &SInventoryGrid::OnSlotUnhovered)
		.OnSlotDrop(this, &SInventoryGrid::OnSlotDrop);

		SlotWidgets.Add(SlotWidget);
		Grid->AddSlot(Col, Row)[SlotWidget.ToSharedRef()];
	}
}

void SInventoryGrid::OnSlotClicked(const int32 SlotIndex)
{
	SelectedSlotIndex = (SelectedSlotIndex == SlotIndex) ? INDEX_NONE : SlotIndex;
}

void SInventoryGrid::OnSlotHovered(const int32 SlotIndex)
{
	if (const auto HoveredSlot = SlotWidgets[SlotIndex]; HoveredSlot.IsValid() && HoveredSlot->IsSlotSet())
	{
		const auto AbsolutePos = HoveredSlot->GetCachedGeometry().GetAbsolutePosition();
		const auto Offset = FVector2D(HoveredSlot->GetCachedGeometry().GetAbsoluteSize().X, 0);
		InfoPanelPosition = GetCachedGeometry().AbsoluteToLocal(AbsolutePos + Offset);

		const auto ItemName = HoveredSlot->GetSlotData().Item->Name;
		const auto ItemDesc = HoveredSlot->GetSlotData().Item->Description;
		ItemInfoPanel->SetTooltip(ItemName, ItemDesc);

		ItemInfoPanel->SetVisibility(EVisibility::HitTestInvisible);
	}
}

void SInventoryGrid::OnSlotUnhovered(int32 SlotIndex) const
{
	ItemInfoPanel->SetVisibility(EVisibility::Collapsed);
}

void SInventoryGrid::OnSlotDrop(const int32 DestinationIndex, const int32 SourceIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("OnSlotDropped from: %d to: %d"), SourceIndex, DestinationIndex);

	FInventorySlotData& SourceSlot = Slots[SourceIndex];
	FInventorySlotData& DestinationSlot = Slots[DestinationIndex];

	Swap(DestinationSlot.Item, SourceSlot.Item);
	Swap(DestinationSlot.bOccupied, SourceSlot.bOccupied);

	SlotWidgets[DestinationIndex]->UpdateSlotData(DestinationSlot);
	SlotWidgets[SourceIndex]->UpdateSlotData(SourceSlot);
	SlotWidgets[DestinationIndex]->Invalidate(EInvalidateWidgetReason::Paint);
	SlotWidgets[SourceIndex]->Invalidate(EInvalidateWidgetReason::Paint);
}

bool SInventoryGrid::IsSlotSelected(const int32 SlotIndex) const
{
	return SelectedSlotIndex == SlotIndex;
}

void SInventoryGrid::SetItemAtSlot(const int32 SlotIndex, const FInventoryItem& Item)
{
	if (!Slots.IsValidIndex(SlotIndex)) return;

	Slots[SlotIndex].bOccupied = true;
	Slots[SlotIndex].Item = Item;
	SlotWidgets[SlotIndex]->UpdateSlotData(Slots[SlotIndex]);
}

void SInventoryGrid::ClearSlot(int32 SlotIndex)
{
	if (!Slots.IsValidIndex(SlotIndex)) return;

	Slots[SlotIndex].bOccupied = false;
	Slots[SlotIndex].Item.Reset();
	SlotWidgets[SlotIndex]->UpdateSlotData(Slots[SlotIndex]);
}

FReply SInventoryGrid::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();
	int32 NewIndex = SelectedSlotIndex == INDEX_NONE ? 0 : SelectedSlotIndex;

	if (Key == EKeys::Right) NewIndex += 1;
	else if (Key == EKeys::Left) NewIndex -= 1;
	else if (Key == EKeys::Down) NewIndex += Columns;
	else if (Key == EKeys::Up) NewIndex -= Columns;
	else return FReply::Unhandled();

	if (Slots.IsValidIndex(NewIndex))
	{
		SelectedSlotIndex = NewIndex;
	}

	return FReply::Handled();
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
