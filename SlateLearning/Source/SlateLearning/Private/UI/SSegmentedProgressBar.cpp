// Fill out your copyright notice in the Description page of Project Settings.


#include "SSegmentedProgressBar.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSegmentedProgressBar::Construct(const FArguments& InArgs)
{
	NumSegments = InArgs._NumSegments;
	Percent = InArgs._Percent;
	GapSize = InArgs._GapSize;
	FillColor = InArgs._FillColor;
	BackgroundColor = InArgs._BackgroundColor;
}

int32 SSegmentedProgressBar::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                     const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                                     int32 LayerId,
                                     const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 RetLayer = LayerId;

	const FVector2D WidgetSize = AllottedGeometry.GetLocalSize();
	const float MaxTotalGaps = WidgetSize.X * 0.5f; // gaps can never exceed 50% of width
	const float ClampedGapSize = FMath::Min(GapSize, MaxTotalGaps / FMath::Max(NumSegments - 1, 1));
	const float TotalGaps = ClampedGapSize * (NumSegments - 1);
	const float SegmentWidth = (WidgetSize.X - TotalGaps) / NumSegments;
	const float CurrentPercent = FMath::Clamp(Percent.Get(), 0.0f, 1.0f);
	const float FilledAmount = CurrentPercent * NumSegments;

	const FShear2D Shear = FShear2D(FVector2D(ShearAmount, 0.0f));
	const FSlateRenderTransform ShearTransform(Shear);

	for (int32 i = 0; i < NumSegments; i++)
	{
		const float SegmentX = i * (SegmentWidth + ClampedGapSize);

		const float SegmentFill = FMath::Clamp(FilledAmount - i, 0.0f, 1.0f);

		FSlateDrawElement::MakeBox(
			OutDrawElements,
			RetLayer,
			AllottedGeometry.ToPaintGeometry(
				FVector2D(SegmentWidth, WidgetSize.Y),
				FSlateLayoutTransform(FVector2D(SegmentX, 0.0f)),
				ShearTransform),
			FCoreStyle::Get().GetBrush("WhiteBrush"),
			ESlateDrawEffect::None,
			BackgroundColor
		);

		if (SegmentFill > 0.0f)
		{
			FSlateDrawElement::MakeBox(
				OutDrawElements,
				RetLayer++,
				AllottedGeometry.ToPaintGeometry(
					FVector2D(SegmentWidth * SegmentFill, WidgetSize.Y),
					FSlateLayoutTransform(FVector2D(SegmentX, 0.0f)),
					ShearTransform),
				FCoreStyle::Get().GetBrush("WhiteBrush"),
				ESlateDrawEffect::None,
				FillColor
			);
		}
	}

	return RetLayer;
}

FVector2D SSegmentedProgressBar::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D(300, 32);
}

void SSegmentedProgressBar::SetNumSegments(int32 InNumSegments)
{
	NumSegments = InNumSegments;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SSegmentedProgressBar::SetPercent(float InPercent)
{
	Percent = InPercent;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SSegmentedProgressBar::SetGapSize(float InGapSize)
{
	GapSize = InGapSize;
	Invalidate(EInvalidateWidgetReason::Layout);
}

void SSegmentedProgressBar::SetFillColor(FLinearColor InFillColor)
{
	FillColor = InFillColor;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SSegmentedProgressBar::SetBackgroundColor(FLinearColor InBackgroundColor)
{
	BackgroundColor = InBackgroundColor;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SSegmentedProgressBar::SetShearAmount(float InShearAmount)
{
	ShearAmount = InShearAmount;
	Invalidate(EInvalidateWidgetReason::Paint);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
