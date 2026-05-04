#pragma once

#include "CoreMinimal.h"
#include "InventoryTypes.h"

class FInventoryDragDropOp : public FDragDropOperation
{
public:
	DRAG_DROP_OPERATOR_TYPE(FInventoryDragDropOp, FDragDropOperation)

	FInventoryItem ItemToTransfer;
	int32 FromSlot;
	FVector2D SlotSize;

	static TSharedRef<FInventoryDragDropOp> New(const int32 InFromSlot, const FInventoryItem& InItem,
	                                            const FVector2D InSlotSize)
	{
		TSharedRef<FInventoryDragDropOp> Op = MakeShared<FInventoryDragDropOp>();
		Op->ItemToTransfer = InItem;
		Op->SlotSize = InSlotSize;
		Op->FromSlot = InFromSlot;
		Op->bCreateNewWindow = false;
		UE_LOG(LogTemp, Warning, TEXT("DragOp New() called"));
		Op->Construct();
		return Op;
	}

	virtual FVector2D GetDecoratorPosition() const override final;

protected:
	virtual TSharedPtr<SWidget> GetDefaultDecorator() const override;
};
