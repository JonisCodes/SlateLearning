// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SInventorySlot.h"

#include "SInventorySlotDecorator.h"
#include "SlateOptMacros.h"
#include "Inventory/FInventoryDragDropOp.h"
#include "Input/Reply.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SInventorySlot::Construct(const FArguments& InArgs)
{
	SlotData = InArgs._SlotData;
	bSelected = InArgs._bSelected;
	OnSlotClickedDelegate = InArgs._OnSlotClicked;
	OnSlotHoveredDelegate = InArgs._OnSlotHovered;
	OnSlotUnHoveredDelegate = InArgs._OnSlotUnhovered;
	OnSlotDropDelegate = InArgs._OnSlotDrop;
}


void SInventorySlot::UpdateSlotData(const FInventorySlotData& NewData)
{
	SlotData = NewData;
}

bool SInventorySlot::IsSlotSet() const
{
	return SlotData.bOccupied;
}

FInventorySlotData SInventorySlot::GetSlotData() const
{
	return SlotData;
}

FVector2D SInventorySlot::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D(64.f, 64.f);
}

int32 SInventorySlot::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                              const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                              int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const FVector2D Size = AllottedGeometry.GetLocalSize();

	// Background
	const auto BgColor = FLinearColor(0.05f, 0.05f, 0.05f, 0.9f);
	FSlateDrawElement::MakeBox(OutDrawElements, LayerId,
	                           AllottedGeometry.ToPaintGeometry(),
	                           FCoreStyle::Get().GetBrush("WhiteBrush"),
	                           ESlateDrawEffect::None, BgColor);
	++LayerId;

	// Hover tint
	if (bHovered)
	{
		FSlateDrawElement::MakeBox(OutDrawElements, LayerId,
		                           AllottedGeometry.ToPaintGeometry(),
		                           FCoreStyle::Get().GetBrush("WhiteBrush"),
		                           ESlateDrawEffect::None,
		                           FLinearColor(1.f, 1.f, 1.f, 0.08f));
		++LayerId;
	}

	// Item block
	if (SlotData.bOccupied && SlotData.Item.IsSet())
	{
		const auto Padding = 8.f;
		FSlateDrawElement::MakeBox(OutDrawElements, LayerId,
		                           AllottedGeometry.ToPaintGeometry(
			                           FVector2D(Size.X - Padding * 2.f, Size.Y - Padding * 2.f),
			                           FSlateLayoutTransform(FVector2D(Padding, Padding))),
		                           FCoreStyle::Get().GetBrush("WhiteBrush"),
		                           ESlateDrawEffect::None,
		                           SlotData.Item->Color);
		++LayerId;
	}

	// Selection border
	if (bSelected.Get())
	{
		FSlateDrawElement::MakeBox(OutDrawElements, LayerId,
		                           AllottedGeometry.ToPaintGeometry(),
		                           FCoreStyle::Get().GetBrush("FocusRectangle"),
		                           ESlateDrawEffect::None,
		                           FLinearColor(1.f, 0.8f, 0.f, 1.f));
		++LayerId;
	}

	return LayerId;
}

FReply SInventorySlot::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnSlotClickedDelegate.ExecuteIfBound(SlotData.SlotIndex);
		return FReply::Handled().DetectDrag(SharedThis(this), EKeys::LeftMouseButton);
	}


	return FReply::Unhandled();
}

void SInventorySlot::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bHovered = true;
	OnSlotHoveredDelegate.ExecuteIfBound(SlotData.SlotIndex);
}

void SInventorySlot::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	bHovered = false;
	OnSlotUnHoveredDelegate.ExecuteIfBound(SlotData.SlotIndex);
}

FReply SInventorySlot::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (SlotData.bOccupied)
	{
		UE_LOG(LogTemp, Warning, TEXT("Drag detected on: %i"), SlotData.SlotIndex);
		const TSharedRef<FInventoryDragDropOp> DragOp = FInventoryDragDropOp::New(
			SlotData.SlotIndex,
			*SlotData.Item,
			GetDesiredSize());

		return FReply::Handled().BeginDragDrop(DragOp);
	}

	return FReply::Unhandled();
}

FReply SInventorySlot::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	// the slot that receives the drop
	const auto DragDropOp = DragDropEvent.GetOperationAs<FInventoryDragDropOp>();

	if (!DragDropOp.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("DragDropOp not valid on Drop"));
		return FReply::Unhandled();
	}

	OnSlotDropDelegate.ExecuteIfBound(SlotData.SlotIndex, DragDropOp->FromSlot);
	return FReply::Handled().EndDragDrop();
}

FReply SInventorySlot::OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	return SLeafWidget::OnDragOver(MyGeometry, DragDropEvent);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
