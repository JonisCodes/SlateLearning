// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FComboBoxItem.h"
#include "Widgets/SLeafWidget.h"

struct FComboBoxItem;
/**
 * 
 */
class SLATELEARNING_API SComboHeader : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SComboHeader)
			: _BackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.5f))
			  , _DesiredWidth(300.f)
		{
		}

		SLATE_ARGUMENT(FLinearColor, BackgroundColor)
		SLATE_ARGUMENT(float, DesiredWidth)
		SLATE_EVENT(FSimpleDelegate, OnHeaderClicked)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	                      FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	                      bool bParentEnabled) const override;

	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDoubleClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	void ToggleHeader(bool bNewValue);
	FString GetSelectedElement() const;
	void SetSelectedItem(const FComboBoxItem& NewItem);

	bool bIsOpen = false;

	FLinearColor BackgroundColor;
	float DesiredWidth;
	FSimpleDelegate OnHeaderClicked;

private:
	FComboBoxItem SelectedItem;
};
