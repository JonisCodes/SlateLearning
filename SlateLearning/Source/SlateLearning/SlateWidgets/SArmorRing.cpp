// Fill out your copyright notice in the Description page of Project Settings.


#include "SArmorRing.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SArmorRing::Construct(const FArguments& InArgs)
{
	ArmorPercent = InArgs._ArmorPercent;
}

int32 SArmorRing::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	const FVector2D Center = AllottedGeometry.GetLocalSize() * 0.5f;
	const float Radius = Center.X - 5.f;
	const int32 Segments = 32;
	
	TArray<FVector2D> BackgroundPoints;
	for (auto i = 0; i <= Segments; ++i)
	{
		float Angle = (2.f * PI * i) / Segments;
		BackgroundPoints.Add(Center + FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)) * Radius);
	}
	
	FSlateDrawElement::MakeLines(OutDrawElements, LayerId,
		AllottedGeometry.ToPaintGeometry(),
		BackgroundPoints,
		ESlateDrawEffect::None,
		FLinearColor(0.15f, 0.15f, 0.15f),
		true, 6.f);
	
	const auto Percent = FMath::Clamp(ArmorPercent.Get(), 0.f, 1.f);
	const auto ArcSegments = FMath::RoundToInt(Segments * Percent);
	
	TArray<FVector2D> ArcPoints;
	for (auto i = 0; i <= ArcSegments; i++)
	{
		auto Angle = (2.f * PI * i) / Segments - PI * 0.5f;
		ArcPoints.Add(Center + FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)) * Radius);
	}
	
	if (ArcPoints.Num() > 1)
	{
		FSlateDrawElement::MakeLines(OutDrawElements, LayerId + 1,
			AllottedGeometry.ToPaintGeometry(),
			ArcPoints,
			ESlateDrawEffect::None,
			FLinearColor(0.f, 0.6f, 1.f),
			true, 6.f);
	}
	
	return LayerId + 1;
}

FVector2D SArmorRing::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D(100.f, 100.f);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
