// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FRadialMenuItem.h"
#include "Widgets/SLeafWidget.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnSegmentSelected, const FRadialMenuItem& SegmentIndex);
DECLARE_DELEGATE_OneParam(FOnSegmentHovered, const FRadialMenuItem& SegmentItem);
DECLARE_DELEGATE_OneParam(FOnSegmentUnhovered, const FRadialMenuItem& SegmentItem);

class SLATELEARNING_API SRadialMenu : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SRadialMenu)
			:
			_NumSegments(5)
			, _OuterRadius(300.f)
			, _InnerRadius(30.f)
			, _SelectionKey(EKeys::E)
		{
		}

		SLATE_ARGUMENT(int32, NumSegments)
		SLATE_ARGUMENT(float, OuterRadius)
		SLATE_ARGUMENT(float, InnerRadius)
		SLATE_ARGUMENT(FKey, SelectionKey)

		SLATE_EVENT(FOnSegmentSelected, OnSegmentSelected)
		SLATE_EVENT(FOnSegmentHovered, OnSegmentHovered)
		SLATE_EVENT(FOnSegmentUnhovered, OnSegmentUnhovered)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	                      FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	                      bool bParentEnabled) const override;
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual bool ComputeVolatility() const override { return true; }
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	// virtual FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	// virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual bool SupportsKeyboardFocus() const override { return true; }

	void AddItem(const FRadialMenuItem& NewItem);
	void ConfirmSelection();

	void SetOuterRadius(const float NewOuterRadius);
	void SetInnerRadius(const float NewInnerRadius);
	void SetNumSegments(const int32 NewNumSegments);
	void SetSelectionKey(const FKey& NewSelectionKey);

	FOnSegmentSelected OnSegmentSelectedDelegate;

protected:
	// Convert angle + radius to a 2D point relative to center
	static FVector2D AngleToPoint(FVector2D Center, float Radius, float AngleRad);

private:
	FOnSegmentHovered OnSegmentHoveredDelegate;
	FOnSegmentUnhovered OnSegmentUnhoveredDelegate;

	int32 NumSegments = 5;
	float OuterRadius = 300.f;
	float InnerRadius = 30.f;
	FKey SelectionKey;

	mutable int32 HoveredSegmentIndex = INDEX_NONE;
	TArray<FRadialMenuItem> Items;
};
