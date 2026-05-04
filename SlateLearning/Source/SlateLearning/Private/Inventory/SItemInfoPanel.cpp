// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SItemInfoPanel.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SItemInfoPanel::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 1.f))
		.BorderImage(FCoreStyle::Get().GetBrush("GenericWhiteBox"))
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.Padding(FMargin(4.f, 2.f))
			[
				SNew(STextBlock)
				.Text_Lambda([this]()
				{
					return Name;
				})
				.ColorAndOpacity(FLinearColor::White)
			]

			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Padding(FMargin(4.f, 2.f))
			[
				SNew(STextBlock)
				.Text_Lambda([this]()
				{
					return Description;
				})
				.ColorAndOpacity(FLinearColor(0.8f, 0.8f, 0.8f))
			]
		]
	];
}

void SItemInfoPanel::SetTooltip(const FText& InName, const FText& InDescription)
{
	Name = InName;
	Description = InDescription;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
