// Fill out your copyright notice in the Description page of Project Settings.


#include "DraggablePanel/STitlebar.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

static const FSlateColorBrush NoBrush(FLinearColor::Transparent);

void STitlebar::Construct(const FArguments& InArgs)
{
	OnPanelClosed = InArgs._OnPanelClosed;
	OnPanelMove = InArgs._OnPanelMove;

	BorderColor = InArgs._BorderColor;
	BorderBrush = InArgs._BorderBrush;
	TextPadding = InArgs._TextPadding;
	TextColor = InArgs._TextColor;
	Text = InArgs._Text;
	Font = InArgs._Font;
	CloseButtonSize = InArgs._CloseButtonSize;
	ButtonContentPadding = InArgs._ButtonContentPadding;
	ButtonColor = InArgs._ButtonColor;
	ButtonTextColor = InArgs._ButtonTextColor;
	ButtonStyle = InArgs._ButtonStyle;

	ChildSlot
	[
		SAssignNew(TitleBorder, SBorder)
		.BorderBackgroundColor(FSlateColor(BorderColor.Get()))
		.BorderImage(&BorderBrush.Get())
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			[
				SAssignNew(TextPaddingWrapper, SBorder)
				.Padding(TextPadding.Get())
				.BorderImage(&NoBrush)
				[
					SAssignNew(TitleTextBlock, STextBlock)
					.Text(Text.Get())
					.ColorAndOpacity(TextColor.Get())
					.Font(Font.Get())
				]
			]

			+ SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			[
				SAssignNew(CloseButtonSizeBox, SBox)
				.WidthOverride(CloseButtonSize.Get().X)
				.HeightOverride(CloseButtonSize.Get().Y)
				[
					SAssignNew(CloseButton, SButton)
					.ButtonColorAndOpacity(FSlateColor(ButtonColor.Get()))
					.ForegroundColor(FSlateColor(ButtonTextColor.Get()))
					.ButtonStyle(&ButtonStyle.Get())
					.ContentPadding(0)
					.OnClicked(this, &STitlebar::HandleCloseClicked)
					[
						SAssignNew(ButtonContentWrapper, SBorder)
						.Padding(ButtonContentPadding.Get())
						.BorderImage(&NoBrush)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Text(INVTEXT("X"))
						]
					]
				]
			]
		]
	];
}

FReply STitlebar::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bIsDragging = true;
	return FReply::Handled().CaptureMouse(SharedThis(this));
}

FReply STitlebar::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!bIsDragging) return FReply::Handled();

	bIsDragging = false;
	return FReply::Handled().ReleaseMouseCapture();
}

FReply STitlebar::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (bIsDragging)
	{
		const float Scale = MyGeometry.GetAccumulatedLayoutTransform().GetScale();
		const FVector2D Delta = MouseEvent.GetCursorDelta() / Scale;
		OnPanelMove.ExecuteIfBound(Delta);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void STitlebar::SetBorderColor(FLinearColor InBorderColor)
{
	BorderColor.Set(InBorderColor);
	if (TitleBorder.IsValid()) TitleBorder->SetBorderBackgroundColor(FSlateColor(InBorderColor));
}

void STitlebar::SetBorderBrush(FSlateBrush InBorderBrush)
{
	BorderBrush.Set(InBorderBrush);
	if (TitleBorder.IsValid()) TitleBorder->SetBorderImage(&BorderBrush.Get());
}

void STitlebar::SetText(FText InText)
{
	Text.Set(InText);
	if (TitleTextBlock.IsValid()) TitleTextBlock->SetText(InText);
}

void STitlebar::SetTextColor(FLinearColor InTextColor)
{
	TextColor.Set(InTextColor);
	if (TitleTextBlock.IsValid()) TitleTextBlock->SetColorAndOpacity(InTextColor);
}

void STitlebar::SetFont(FSlateFontInfo InFont)
{
	Font.Set(InFont);
	if (TitleTextBlock.IsValid()) TitleTextBlock->SetFont(InFont);
}

void STitlebar::SetTextPadding(FMargin InTextPadding)
{
	TextPadding.Set(InTextPadding);
	if (TextPaddingWrapper.IsValid()) TextPaddingWrapper->SetPadding(InTextPadding);
}

void STitlebar::SetCloseButtonSize(FVector2D InCloseButtonSize)
{
	CloseButtonSize.Set(InCloseButtonSize);
	if (CloseButtonSizeBox.IsValid())
	{
		CloseButtonSizeBox->SetWidthOverride(InCloseButtonSize.X);
		CloseButtonSizeBox->SetHeightOverride(InCloseButtonSize.Y);
	}
}

void STitlebar::SetButtonContentPadding(FMargin InButtonContentPadding)
{
	ButtonContentPadding.Set(InButtonContentPadding);
	if (ButtonContentWrapper.IsValid()) ButtonContentWrapper->SetPadding(InButtonContentPadding);
}

void STitlebar::SetButtonColor(FLinearColor InButtonColor)
{
	ButtonColor.Set(InButtonColor);
	if (CloseButton.IsValid()) CloseButton->SetBorderBackgroundColor(FSlateColor(InButtonColor));
}

void STitlebar::SetButtonTextColor(FLinearColor InButtonTextColor)
{
	ButtonTextColor.Set(InButtonTextColor);
	if (CloseButton.IsValid()) CloseButton->SetForegroundColor(FSlateColor(InButtonTextColor));
}

void STitlebar::SetButtonStyle(FButtonStyle InButtonStyle)
{
	ButtonStyle.Set(InButtonStyle);
	if (CloseButton.IsValid()) CloseButton->SetButtonStyle(&ButtonStyle.Get());
}


FVector2D STitlebar::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D(300.f, 42.f);
}

FReply STitlebar::HandleCloseClicked() const
{
	OnPanelClosed.ExecuteIfBound();
	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
