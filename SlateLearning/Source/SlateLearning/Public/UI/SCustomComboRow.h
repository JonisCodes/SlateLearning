// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FComboBoxItem.h"
#include "Widgets/SLeafWidget.h"

struct FComboBoxItem;
/**
 * 
 */

DECLARE_DELEGATE_OneParam(FOnRowClicked, const FComboBoxItem&);

class SLATELEARNING_API SCustomComboRow : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SCustomComboRow)
			: _Font(FCoreStyle::Get().GetFontStyle("NormalText"))
		{
		}

		SLATE_ARGUMENT(FComboBoxItem, Item)
		SLATE_ARGUMENT(FSlateFontInfo, Font)
		SLATE_EVENT(FOnRowClicked, OnRowClicked)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	                      FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	                      bool bParentEnabled) const override;

	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;

	FComboBoxItem Item;
	FOnRowClicked OnRowClicked;
	FSlateFontInfo Font;
};
