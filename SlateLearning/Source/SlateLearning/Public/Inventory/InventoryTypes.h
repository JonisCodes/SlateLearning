// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "InventoryTypes.generated.h"

/**
 * 
 */
UENUM()
enum class EItemType : uint8
{
	None,
	Weapon,
	Consumable,
	Armor
};

class FInventoryItem
{
public:
	FText Name;
	FText Description;
	EItemType Type = EItemType::None;
	FLinearColor Color = FLinearColor::White;
};

struct FInventorySlotData
{
	int32 SlotIndex = INDEX_NONE;
	bool bOccupied = false;
	TOptional<FInventoryItem> Item;
};
