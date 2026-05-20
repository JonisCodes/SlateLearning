// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"

/**
 * 
 */
class SLATELEARNING_API SStatBar : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SStatBar)
			: _HighColor(FLinearColor::Green)
			  , _MidColor(FLinearColor::Yellow)
			  , _LowColor(FLinearColor::Red)
			  , _GhostColor(FLinearColor(0.8f, 0.8f, 0.2f, 0.8f))
			  , _BackgroundColor(FLinearColor::Black)
			  , _ForegroundColor(FLinearColor::White)
			  , _bShowGhostBar(true)
			  , _DrainSpeed(1.f)
		{
		}

		SLATE_ARGUMENT(FLinearColor, HighColor)
		SLATE_ARGUMENT(FLinearColor, MidColor)
		SLATE_ARGUMENT(FLinearColor, LowColor)
		SLATE_ARGUMENT(FLinearColor, GhostColor)
		SLATE_ARGUMENT(FLinearColor, BackgroundColor)
		SLATE_ARGUMENT(FLinearColor, ForegroundColor)
		SLATE_ARGUMENT(bool, bShowGhostBar)
		SLATE_ARGUMENT(float, DrainSpeed)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FLinearColor GetFillColor() const;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	                      FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	                      bool bParentEnabled) const override;

	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual bool ComputeVolatility() const override { return true; }

	void SetCurrentValue(const int32 NewCurrent);
	void SetHighColor(const FLinearColor NewHighColor);
	void SetMidColor(const FLinearColor NewMidColor);
	void SetLowColor(const FLinearColor NewLowColor);
	void SetGhostColor(const FLinearColor NewGhostColor);
	void SetBackgroundColor(const FLinearColor NewBackgroundColor);
	void SetShowGhostBar(const bool NewShow);
	void SetDrainSpeed(const float NewDrainSpeed);

private:
	float CurrentValue = 100;
	float MaxValue = 100;
	mutable float GhostValue = 100;

	FCurveSequence DrainSequence;
	FCurveHandle DrainHandle;
	mutable float GhostStartValue = 0.f;

	FLinearColor HighColor;
	FLinearColor MidColor;
	FLinearColor LowColor;
	FLinearColor GhostColor;
	FLinearColor BackgroundColor;
	bool bShowGhostBar;
	float DrainSpeed;
};
