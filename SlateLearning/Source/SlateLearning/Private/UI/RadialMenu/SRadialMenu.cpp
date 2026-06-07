// Fill out your copyright notice in the Description page of Project Settings.


#include "RadialMenu/SRadialMenu.h"

#include "SlateOptMacros.h"
#include "Components/SlateWrapperTypes.h"
#include "Fonts/FontMeasure.h"
#include "Slate/SlateBrushAsset.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SRadialMenu::Construct(const FArguments& InArgs)
{
	NumSegments = InArgs._NumSegments;
	OuterRadius = InArgs._OuterRadius;
	InnerRadius = InArgs._InnerRadius;
	SelectionKey = InArgs._SelectionKey;

	OnSegmentSelectedDelegate = InArgs._OnSegmentSelected;
	OnSegmentHoveredDelegate = InArgs._OnSegmentHovered;
	OnSegmentUnhoveredDelegate = InArgs._OnSegmentUnhovered;
	SetNumSegments(NumSegments);
}

int32 SRadialMenu::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
                           FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
                           bool bParentEnabled) const
{
	int32 RetLayer = LayerId;

	const FVector2D Center = AllottedGeometry.GetLocalSize() / 2.0f;
	const float SegmentAngle = 2.f * PI / NumSegments;

	const FSlateFontInfo FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", 14);

	for (int32 i = 0; i < NumSegments; i++)
	{
		const int32 Steps = FMath::Max(16, FMath::RoundToInt(OuterRadius * 0.2f));

		const float StartAngle = i * SegmentAngle - (PI / 2.f);
		const float EndAngle = StartAngle + SegmentAngle;

		// // Fill
		TArray<FSlateVertex> Verts;
		TArray<SlateIndex> Indices;

		// const FLinearColor DrawColor = (i == HoveredSegmentIndex)
		// 	                               ? FLinearColor::LerpUsingHSV(Items[i].Color, FLinearColor::White, 0.3f)
		// 	                               : Items[i].Color;
		const FVector2D UV(0, 0);

		for (int32 Step = 0; Step <= Steps; Step++)
		{
			const float T = static_cast<float>(Step) / Steps;
			const float Angle = FMath::Lerp(StartAngle, EndAngle, T);

			const FVector2D OuterPoint = AngleToPoint(Center, OuterRadius, Angle);
			const FVector2D InnerPoint = AngleToPoint(Center, InnerRadius, Angle);

			const FVector2D AbsoluteOuter = AllottedGeometry.LocalToAbsolute(OuterPoint);
			const FVector2D AbsoluteInner = AllottedGeometry.LocalToAbsolute(InnerPoint);

			FSlateVertex OuterVerts;
			OuterVerts.Position = FVector2f(AbsoluteOuter.X, AbsoluteOuter.Y);
			OuterVerts.TexCoords[0] = OuterVerts.TexCoords[1] = OuterVerts.TexCoords[2] = OuterVerts.TexCoords[3] = UV.
				X;
			// OuterVerts.Color = DrawColor.ToFColor(true);


			FSlateVertex InnerVert;
			InnerVert.Position = FVector2f(AbsoluteInner.X, AbsoluteInner.Y);
			InnerVert.TexCoords[0] = InnerVert.TexCoords[1] = InnerVert.TexCoords[2] = InnerVert.TexCoords[3] = UV.X;
			// InnerVert.Color = DrawColor.ToFColor(true);

			Verts.Add(OuterVerts);
			Verts.Add(InnerVert);
		}

		for (int32 Step = 0; Step < Steps; Step++)
		{
			const SlateIndex OuterCurrent = Step * 2;
			const SlateIndex InnerCurrent = Step * 2 + 1;
			const SlateIndex OuterNext = Step * 2 + 2;
			const SlateIndex InnerNext = Step * 2 + 3;

			// Triangle 1
			Indices.Add(OuterCurrent);
			Indices.Add(OuterNext);
			Indices.Add(InnerCurrent);

			// Triangle 2
			Indices.Add(InnerCurrent);
			Indices.Add(OuterNext);
			Indices.Add(InnerNext);
		}


		FSlateResourceHandle ResourceHandle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(
			Items[i].Brush);

		FSlateDrawElement::MakeCustomVerts(
			OutDrawElements,
			RetLayer++,
			ResourceHandle,
			Verts,
			Indices,
			nullptr, 0, 0,
			ESlateDrawEffect::None
		);

		// Build Label
		const float MidAngle = StartAngle + (SegmentAngle / 2.f);
		const float LabelRadius = (OuterRadius + InnerRadius) / 2.f;
		const FVector2D LabelPos = AngleToPoint(Center, LabelRadius, MidAngle);

		const TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->
			GetFontMeasureService();
		const FVector2D TextSize = FontMeasure->Measure(Items[i].Label, FontInfo);

		FSlateDrawElement::MakeText(
			OutDrawElements,
			RetLayer++,
			AllottedGeometry.ToPaintGeometry(
				TextSize,
				FSlateLayoutTransform(LabelPos - TextSize / 2.f)),
			Items[i].Label,
			FontInfo,
			ESlateDrawEffect::None,
			FLinearColor::Black
		);

		// Build outer arc points
		TArray<FVector2D> OuterArcPoints;
		TArray<FVector2D> InnerArcPoints;

		for (int32 Step = 0; Step <= Steps; Step++)
		{
			const float T = static_cast<float>(Step) / Steps;

			const float OuterAngle = FMath::Lerp(StartAngle, EndAngle, T);
			const float InnerAngle = FMath::Lerp(EndAngle, StartAngle, T);

			const FVector2D OuterPoint = AngleToPoint(Center, OuterRadius, OuterAngle);
			const FVector2D InnerPoint = AngleToPoint(Center, InnerRadius, InnerAngle);

			InnerArcPoints.Add(InnerPoint);
			OuterArcPoints.Add(OuterPoint);
		}

		TArray<FVector2D> SegmentPoints;
		SegmentPoints.Append(OuterArcPoints);
		SegmentPoints.Append(InnerArcPoints);
		SegmentPoints.Add(OuterArcPoints[0]);

		const float DPIScale = AllottedGeometry.GetAccumulatedLayoutTransform().GetScale();

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			RetLayer++,
			AllottedGeometry.ToPaintGeometry(),
			SegmentPoints,
			ESlateDrawEffect::None,
			FLinearColor::Black,
			true,
			2.f * DPIScale
		);
	}

	return RetLayer;
}

FVector2D SRadialMenu::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2d(300.f, 300.f);
}

FReply SRadialMenu::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const FVector2D LocalPos = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	const FVector2D FromCenter = LocalPos - MyGeometry.GetLocalSize() / 2;

	const float AngleRad = FMath::Atan2(FromCenter.Y, FromCenter.X);

	float NormalizedAngle = AngleRad + PI / 2.f;
	if (NormalizedAngle < 0.f)
	{
		NormalizedAngle += 2.f * PI;
	}

	const float SegmentAngle = 2.f * PI / NumSegments;

	if (const int32 NewHoveredIndex = FMath::FloorToInt(NormalizedAngle / SegmentAngle) % NumSegments;
		NewHoveredIndex != HoveredSegmentIndex)
	{
		if (Items.IsValidIndex(HoveredSegmentIndex))
		{
			// call unhovered
			OnSegmentUnhoveredDelegate.ExecuteIfBound(Items[HoveredSegmentIndex]);
		}

		HoveredSegmentIndex = NewHoveredIndex;

		// call hovered on new hovered segment
		OnSegmentHoveredDelegate.ExecuteIfBound(Items[HoveredSegmentIndex]);
		Invalidate(EInvalidateWidgetReason::Paint);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

// FReply SRadialMenu::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
// {
// 	if (InKeyEvent.GetKey() == SelectionKey)
// 	{
// 		const int32 SelectedSegmentIndex = HoveredSegmentIndex;
//
// 		OnSegmentSelectedDelegate.ExecuteIfBound(Items[SelectedSegmentIndex]);
// 		return FReply::Handled();
// 	}
//
// 	return FReply::Unhandled();
// }
//
// FReply SRadialMenu::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
// {
// 	return SLeafWidget::OnKeyDown(MyGeometry, InKeyEvent);
// }

FReply SRadialMenu::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled().SetUserFocus(SharedThis(this), EFocusCause::Mouse);
}

void SRadialMenu::AddItem(const FRadialMenuItem& NewItem)
{
	NumSegments++;
	Items.Add(NewItem);
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SRadialMenu::ConfirmSelection()
{
	if (!Items.IsValidIndex(HoveredSegmentIndex)) return;

	OnSegmentSelectedDelegate.ExecuteIfBound(Items[HoveredSegmentIndex]);
}

void SRadialMenu::SetOuterRadius(const float NewOuterRadius)
{
	OuterRadius = NewOuterRadius;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SRadialMenu::SetInnerRadius(const float NewInnerRadius)
{
	InnerRadius = NewInnerRadius;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SRadialMenu::SetNumSegments(const int32 NewNumSegments)
{
	NumSegments = NewNumSegments;

	Items.Empty();
	for (int32 i = 0; i < NumSegments; i++)
	{
		FRadialMenuItem Item;

		Item.Color = FLinearColor::Gray;
		Item.HoveredColor = FLinearColor::Gray + FLinearColor(0.2f, 0.2, 0.2f);
		Item.Brush = *FCoreStyle::Get().GetBrush("WhiteBrush");
		Item.Label = FText::FromString(FString::Printf(TEXT("Item: %d"), i));
		Items.Add(Item);
	}

	Invalidate(EInvalidateWidgetReason::Paint);
}

void SRadialMenu::SetSelectionKey(const FKey& NewSelectionKey)
{
	SelectionKey = NewSelectionKey;
}

FVector2D SRadialMenu::AngleToPoint(FVector2D Center, const float Radius, const float AngleRad)
{
	return FVector2D(
		Center.X + FMath::Cos(AngleRad) * Radius,
		Center.Y + FMath::Sin(AngleRad) * Radius
	);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
