// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"

/**
 * 
 */
class SLATELEARNING_API SArmorRing : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SArmorRing)
		{
		}

		SLATE_ATTRIBUTE(float, ArmorPercent)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
		int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual bool ComputeVolatility() const override { return true; }
	
private:
	TAttribute<float> ArmorPercent;
	
	// Cached geometry only recalculate when percent changes
	mutable TArray<FVector2D> CachedBackgroundPoints;
	mutable TArray<FVector2D> CachedArcPoints;
	mutable float CachedPercent = -1.f;
	mutable FVector2D CachedSize = FVector2D::ZeroVector;
	
	void RebuildGeometry(const FGeometry& AllottedGeometry) const;
};
