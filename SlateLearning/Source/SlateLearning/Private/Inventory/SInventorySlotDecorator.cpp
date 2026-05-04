// Fill out your copyright notice in the Description page of Project Settings.


#include "SInventorySlotDecorator.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SInventorySlotDecorator::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBox)
		.HeightOverride(InArgs._SlotSize.Get().Y)
		.WidthOverride(InArgs._SlotSize.Get().X)
		[
			SNew(SBorder)
			.BorderBackgroundColor(InArgs._SlotItem.Get().Color)
			.BorderImage(FCoreStyle::Get().GetBrush("GenericWhiteBox"))
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
