// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"

/**
 * 
 */
class SLATELEARNING_API SSegmentedProgressBar : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SSegmentedProgressBar)
			: _NumSegments(10)
			  , _Percent(1.0f)
			  , _GapSize(2.0f)
			  , _FillColor(FLinearColor::Green)
			  , _BackgroundColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f))
			  , _ShearAmount(0.2f)
		{
		}

		SLATE_ARGUMENT(int32, NumSegments)
		SLATE_ATTRIBUTE(float, Percent)
		SLATE_ARGUMENT(float, GapSize)
		SLATE_ARGUMENT(FLinearColor, FillColor)
		SLATE_ARGUMENT(FLinearColor, BackgroundColor)
		SLATE_ARGUMENT(float, ShearAmount)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	                      FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	                      bool bParentEnabled) const override;
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;

	void SetNumSegments(int32 InNumSegments);
	void SetPercent(float InPercent);
	void SetGapSize(float InGapSize);
	void SetFillColor(FLinearColor InFillColor);
	void SetBackgroundColor(FLinearColor InBackgroundColor);
	void SetShearAmount(float InShearAmount);

private:
	int32 NumSegments;
	TAttribute<float> Percent;
	float GapSize;
	FLinearColor FillColor;
	FLinearColor BackgroundColor;
	float ShearAmount;
};
