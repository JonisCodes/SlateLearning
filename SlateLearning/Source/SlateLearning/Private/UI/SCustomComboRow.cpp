// Fill out your copyright notice in the Description page of Project Settings.


#include "SCustomComboRow.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCustomComboRow::Construct(const FArguments& InArgs)
{
	Item = InArgs._Item;
	OnRowClicked = InArgs._OnRowClicked;
}

int32 SCustomComboRow::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                               const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                               int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const FSlateFontInfo FontInfo(
		FCoreStyle::Get().GetFontStyle("NormalText").CompositeFont,
		14
	);

	const FVector2D LocalSize = AllottedGeometry.GetLocalSize();
	constexpr float TextPadding = 8.f;

	FSlateDrawElement::MakeText(
		OutDrawElements,
		LayerId++,
		AllottedGeometry.ToPaintGeometry(
			FVector2D(LocalSize.X - TextPadding * 2.f, LocalSize.Y),
			FSlateLayoutTransform(FVector2D(TextPadding, 0.f))),
		Item.DisplayName.ToString(),
		FontInfo,
		ESlateDrawEffect::None,
		FLinearColor::Black
	);
	return LayerId;
}

FVector2D SCustomComboRow::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D(200.f, 32.f);
}

FReply SCustomComboRow::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	OnRowClicked.ExecuteIfBound(Item);
	return FReply::Handled();
}

FReply SCustomComboRow::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	return OnMouseButtonDown(InMyGeometry, InMouseEvent);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
