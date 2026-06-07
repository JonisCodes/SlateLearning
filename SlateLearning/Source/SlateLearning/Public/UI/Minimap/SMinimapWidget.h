// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"

class UMinimapSubsystem;
/**
 * 
 */
class SLATELEARNING_API SMinimapWidget : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SMinimapWidget)
			: _MinimapSize(FVector2D(200.f, 200.f))
			  , _MinimapSubsystem(nullptr)
			  , _MapRadius(5000.f)
			  , _BackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.5f))
			  , _MapRenderTarget()
			  , _MapMin(FVector2D(-5000.f, -5000.f))
			  , _MapMax(FVector2D(5000.f, 5000.f))
		{
		}

		SLATE_ARGUMENT(FVector2D, MinimapSize)
		SLATE_ARGUMENT(UMinimapSubsystem*, MinimapSubsystem)
		SLATE_ARGUMENT(float, MapRadius);
		SLATE_ARGUMENT(FLinearColor, BackgroundColor);
		SLATE_ARGUMENT(UTextureRenderTarget2D*, MapRenderTarget);
		SLATE_ARGUMENT(FVector2D, MapMin)
		SLATE_ARGUMENT(FVector2D, MapMax)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	const static FVector2D WorldToMinimap(const FVector& WorldPos, const FVector& PlayerPos,
	                                      const FVector2d& MinimapSize, float MapRadius);
	void SetMinimapSize(const FVector2D& NewMinimapSize);
	void SetMapRadius(float NewMapRadius);
	void SetBackgroundColor(FLinearColor NewBackgroundColor);
	void SetRenderTarget(UTextureRenderTarget2D* NewRenderTarget);
	void SetMapMin(const FVector2D& NewMapMin);
	void SetMapMax(const FVector2D& NewMapMax);

protected:
	virtual bool ComputeVolatility() const override { return true; }

private:
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	                      FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	                      bool bParentEnabled) const override;
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;

	FVector2D MinimapSize;
	TWeakObjectPtr<UMinimapSubsystem> MinimapSubsystem;
	float MapRadius;
	FLinearColor BackgroundColor;
	TWeakObjectPtr<UTextureRenderTarget2D> MapRenderTarget;
	mutable TSharedPtr<FSlateImageBrush> CachedRenderTargetBrush;
	FVector2D MapMin;
	FVector2D MapMax;

	const void DrawMinimapIcon(FSlateWindowElementList& OutDrawElements, int32 LayerId,
	                           const FGeometry& AllottedGeometry, const FVector2D& Position, float Size,
	                           const FLinearColor& Color, UTexture2D* Icon = nullptr) const;
};
