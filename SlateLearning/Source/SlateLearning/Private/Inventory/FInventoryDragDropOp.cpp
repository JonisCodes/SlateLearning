#include "Inventory/FInventoryDragDropOp.h"

#include "SInventorySlotDecorator.h"

FVector2D FInventoryDragDropOp::GetDecoratorPosition() const
{
	const auto CursorPos = FSlateApplication::Get().GetCursorPos();
	return CursorPos - (SlotSize * 0.5f);
}

TSharedPtr<SWidget> FInventoryDragDropOp::GetDefaultDecorator() const
{
	return SNew(SBox)
		[
			SNew(SInventorySlotDecorator)
			.Slot(FromSlot)
			.SlotItem(ItemToTransfer)
			.SlotSize(SlotSize)
		];
}
