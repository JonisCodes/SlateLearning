// Fill out your copyright notice in the Description page of Project Settings.


#include "SArmorRing.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SArmorRing::Construct(const FArguments& InArgs)
{
	ArmorPercent = InArgs._ArmorPercent;
	CachedBackgroundPoints.Reserve(33);
	CachedArcPoints.Reserve(33);
}

int32 SArmorRing::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	RebuildGeometry(AllottedGeometry);

	if (const FSlateRect WidgetRect = AllottedGeometry.GetLayoutBoundingRect(); !FSlateRect::DoRectanglesIntersect(WidgetRect, MyCullingRect))
	{
		return LayerId;
	}
	
	// background
	FSlateDrawElement::MakeLines(OutDrawElements, LayerId,
		AllottedGeometry.ToPaintGeometry(),
		CachedBackgroundPoints,
		ESlateDrawEffect::None,
		FLinearColor(0.15f, 0.15f, 0.15f),
		true,  6.f);
	
	// arc
	if (CachedArcPoints.Num() > 1)
	{
		FSlateDrawElement::MakeLines(OutDrawElements, LayerId + 1,
			AllottedGeometry.ToPaintGeometry(),
			CachedArcPoints,
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

void SArmorRing::RebuildGeometry(const FGeometry& AllottedGeometry) const
{
	const FVector2D LocalSize = AllottedGeometry.GetLocalSize();
	const auto Percent = FMath::Clamp(ArmorPercent.Get(), 0.f, 1.f);
	
	// rebuild if size or percent changed
	if (LocalSize.Equals(CachedSize, 0.1f) &&
		FMath::IsNearlyEqual(Percent, CachedPercent, 0.001f))
	{
		// Cache is still valid here
		return; 
	}
	
	UE_LOG(LogTemp, Warning, TEXT("SArmorRing: Rebuilding geometry"));
	
	CachedSize = LocalSize;
	CachedPercent = Percent;
	
	const FVector2D Center = LocalSize * 0.5f;
	const float Radius = Center.X - 5.f;
	const int32 Segments = 32;
	
	// Rebuild background circle
	CachedBackgroundPoints.Reset();
	for (int32 i = 0; i <= Segments; i++)
	{
		const auto Angle = (2.f * PI * i) / Segments;
		CachedBackgroundPoints.Add(Center + 
			FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)) * Radius);
	}
	
	// Rebuild arc
	const int32 ArcSegments = FMath::RoundToInt(Segments * Percent);
	CachedArcPoints.Reset();
	for (int32 i = 0; i <= ArcSegments; i++)
	{
		const auto Angle = (2.f * PI * i) / Segments - PI * 0.5f;
		CachedArcPoints.Add(Center +
			FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)) * Radius);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
