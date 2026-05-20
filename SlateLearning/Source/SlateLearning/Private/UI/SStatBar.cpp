// Fill out your copyright notice in the Description page of Project Settings.


#include "SStatBar.h"

#include "SlateOptMacros.h"
#include "VectorTypes.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SStatBar::Construct(const FArguments& InArgs)
{
	HighColor = InArgs._HighColor;
	MidColor = InArgs._MidColor;
	LowColor = InArgs._LowColor;
	GhostColor = InArgs._GhostColor;
	BackgroundColor = InArgs._BackgroundColor;
	bShowGhostBar = InArgs._bShowGhostBar;
	DrainSpeed = InArgs._DrainSpeed;

	DrainSequence = FCurveSequence();
	DrainHandle = DrainSequence.AddCurve(0.f, DrainSpeed, ECurveEaseFunction::CubicInOut);
}

FLinearColor SStatBar::GetFillColor() const
{
	if (const auto Percent = CurrentValue / MaxValue; Percent > 0.5f)
		return HighColor;
	else if (Percent > 0.25f)
		return MidColor;

	return LowColor;
}

int32 SStatBar::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
                        FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
                        bool bParentEnabled) const
{
	int32 RetLayer = LayerId;

	const float TotalWidth = AllottedGeometry.GetLocalSize().X;
	const float FillWidth = TotalWidth * (CurrentValue / static_cast<float>(MaxValue));


	// background bar
	FSlateDrawElement::MakeBox(
		OutDrawElements,
		RetLayer++,
		AllottedGeometry.ToPaintGeometry(
			FVector2D(TotalWidth, AllottedGeometry.GetLocalSize().Y),
			FSlateLayoutTransform(FVector2d::ZeroVector)),
		FCoreStyle::Get().GetBrush("WhiteBrush"),
		ESlateDrawEffect::None,
		BackgroundColor);

	//ghost bar
	if (bShowGhostBar)
	{
		if (DrainSequence.IsPlaying())
		{
			GhostValue = FMath::Lerp(GhostStartValue, CurrentValue, DrainHandle.GetLerp());
		}

		const float GhostWidth = TotalWidth * (GhostValue / static_cast<float>(MaxValue));

		FSlateDrawElement::MakeBox(
			OutDrawElements,
			RetLayer++,
			AllottedGeometry.ToPaintGeometry(
				FVector2D(GhostWidth, AllottedGeometry.GetLocalSize().Y),
				FSlateLayoutTransform(FVector2d::ZeroVector)),
			FCoreStyle::Get().GetBrush("WhiteBrush"),
			ESlateDrawEffect::None,
			GhostColor);
	}

	// current bar
	FSlateDrawElement::MakeBox(
		OutDrawElements,
		RetLayer++,
		AllottedGeometry.ToPaintGeometry(
			FVector2D(FillWidth, AllottedGeometry.GetLocalSize().Y),
			FSlateLayoutTransform(FVector2d::ZeroVector)),
		FCoreStyle::Get().GetBrush("WhiteBrush"),
		ESlateDrawEffect::None,
		GetFillColor());

	return RetLayer;
}

FVector2D SStatBar::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D(300.f, 24.f);
}

void SStatBar::SetCurrentValue(const int32 NewCurrent)
{
	if (NewCurrent < CurrentValue && bShowGhostBar)
	{
		GhostValue = CurrentValue;

		GhostStartValue = GhostValue;
		DrainSequence.Play(this->AsShared());
	}

	CurrentValue = NewCurrent;
}

void SStatBar::SetHighColor(const FLinearColor NewHighColor)
{
	HighColor = NewHighColor;
}

void SStatBar::SetMidColor(const FLinearColor NewMidColor)
{
	MidColor = NewMidColor;
}

void SStatBar::SetLowColor(const FLinearColor NewLowColor)
{
	LowColor = NewLowColor;
}

void SStatBar::SetGhostColor(const FLinearColor NewGhostColor)
{
	GhostColor = NewGhostColor;
}

void SStatBar::SetBackgroundColor(const FLinearColor NewBackgroundColor)
{
	BackgroundColor = NewBackgroundColor;
}

void SStatBar::SetShowGhostBar(const bool NewShow)
{
	bShowGhostBar = NewShow;
}

void SStatBar::SetDrainSpeed(const float NewDrainSpeed)
{
	DrainSpeed = NewDrainSpeed;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
