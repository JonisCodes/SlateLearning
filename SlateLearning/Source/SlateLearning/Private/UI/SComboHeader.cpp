// Fill out your copyright notice in the Description page of Project Settings.


#include "SComboHeader.h"

#include "SEditorViewportToolBarMenu.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SComboHeader::Construct(const FArguments& InArgs)
{
	BackgroundColor = InArgs._BackgroundColor;
	DesiredWidth = InArgs._DesiredWidth;
	OnHeaderClicked = InArgs._OnHeaderClicked;
	FontColor = InArgs._FontColor;
	ArrowColor = InArgs._ArrowColor;
	HeaderBrush = InArgs._HeaderBrush;
	HeaderFont = InArgs._HeaderFont;
}

int32 SComboHeader::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                            const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                            int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 RetLayer = LayerId;

	// ------- BACKGROUND -------
	FSlateDrawElement::MakeBox(
		OutDrawElements,
		RetLayer++,
		AllottedGeometry.ToPaintGeometry(),
		HeaderBrush,
		ESlateDrawEffect::None,
		BackgroundColor
	);
	// ------- END -------

	// ------- SELECTED ELEMENT TEXT -------
	const FSlateFontInfo FontInfo(
		FCoreStyle::Get().GetFontStyle("NormalText").CompositeFont,
		14
	);

	const FVector2D LocalSize = AllottedGeometry.GetLocalSize();
	constexpr float TextPadding = 8.f;

	FSlateDrawElement::MakeText(
		OutDrawElements,
		RetLayer++,
		AllottedGeometry.ToPaintGeometry(
			FVector2D(LocalSize.X - TextPadding * 2.f, LocalSize.Y),
			FSlateLayoutTransform(FVector2D(TextPadding, 0.f))),
		GetSelectedElement(),
		HeaderFont,
		ESlateDrawEffect::None,
		FontColor
	);
	// ------- END -------

	// ------- ARROW -------
	FSlateDrawElement::MakeText(
		OutDrawElements,
		RetLayer++,
		AllottedGeometry.ToPaintGeometry(
			FVector2D(LocalSize.X - TextPadding - 16.f, LocalSize.Y),
			FSlateLayoutTransform(FVector2D(LocalSize.X - TextPadding - 16.f, 0.f))),
		FText::FromString(bIsOpen ? TEXT("▲") : TEXT("▼")),
		FontInfo,
		ESlateDrawEffect::None,
		ArrowColor
	);
	// ------- END -------

	return RetLayer;
}

FVector2D SComboHeader::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D(DesiredWidth, 40.f);
}

FReply SComboHeader::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	OnHeaderClicked.ExecuteIfBound();
	return FReply::Handled();
}

FReply SComboHeader::OnMouseButtonDoubleClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return OnMouseButtonDown(MyGeometry, MouseEvent);
}

void SComboHeader::ToggleHeader(const bool bNewValue)
{
	bIsOpen = bNewValue;
}

FString SComboHeader::GetSelectedElement() const
{
	return SelectedItem.DisplayName.ToString();
}

void SComboHeader::SetSelectedItem(const FComboBoxItem& NewItem)
{
	SelectedItem = NewItem;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SComboHeader::SetHeaderColor(const FLinearColor NewHeaderColor)
{
	BackgroundColor = NewHeaderColor;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SComboHeader::SetFontColor(const FLinearColor NewFontColor)
{
	FontColor = NewFontColor;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SComboHeader::SetArrowColor(const FLinearColor NewArrowColor)
{
	ArrowColor = NewArrowColor;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SComboHeader::SetHeaderBrush(const FSlateBrush* NewHeaderBrush)
{
	HeaderBrush = NewHeaderBrush;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SComboHeader::SetHeaderFont(const FSlateFontInfo NewHeaderFont)
{
	HeaderFont = NewHeaderFont;
	Invalidate(EInvalidateWidgetReason::Paint);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
