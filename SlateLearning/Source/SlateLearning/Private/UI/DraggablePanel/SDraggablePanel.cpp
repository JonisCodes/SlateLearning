// Fill out your copyright notice in the Description page of Project Settings.


#include "DraggablePanel/SDraggablePanel.h"

#include "SlateOptMacros.h"
#include "DraggablePanel/STitlebar.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDraggablePanel::Construct(const FArguments& InArgs)
{
	OnFocusGained = InArgs._OnFocusGained;
	OnLostFocus = InArgs._OnLostFocus;
	OnDragged = InArgs._OnDragged;
	OnPanelClosed = InArgs._OnPanelClosed;

	TitleBorderColor = InArgs._TitleBorderColor;
	TitleBorderBrush = InArgs._TitleBorderBrush;
	TitleTextPadding = InArgs._TitleTextPadding;
	TitleText = InArgs._TitleText;
	TitleTextColor = InArgs._TitleTextColor;
	TitleFont = InArgs._TitleFont;
	TitleCloseButtonSize = InArgs._TitleCloseButtonSize;
	TitleButtonContentPadding = InArgs._TitleButtonContentPadding;
	TitleButtonColor = InArgs._TitleButtonColor;
	TitleButtonTextColor = InArgs._TitleButtonTextColor;
	TitleCloseButtonStyle = InArgs._TitleCloseButtonStyle;

	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(TitleBar, STitlebar)
			.OnPanelMove(this, &SDraggablePanel::OnTitleBarDragged)
			.BorderColor(TitleBorderColor)
			.BorderBrush(TitleBorderBrush)
			.TextPadding(TitleTextPadding)
			.Text(TitleText)
			.TextColor(TitleTextColor)
			.Font(TitleFont)
			.CloseButtonSize(TitleCloseButtonSize)
			.ButtonContentPadding(TitleButtonContentPadding)
			.ButtonColor(TitleButtonColor)
			.ButtonTextColor(TitleButtonTextColor)
			.ButtonStyle(TitleCloseButtonStyle)
			.OnPanelClosed(this, &SDraggablePanel::ClosePanel)
		]

		+ SVerticalBox::Slot()
		.FillHeight(1.f)
		[
			SAssignNew(ContentArea, SBox)
		]
	];
}

void SDraggablePanel::SetContent(TSharedRef<SWidget> InContent)
{
	ContentArea->SetContent(InContent);
}

void SDraggablePanel::OnTitleBarDragged(const FVector2D Delta)
{
	PanelPosition += Delta;
	SetRenderTransform(FSlateRenderTransform(PanelPosition));
	OnDragged.ExecuteIfBound(PanelPosition);
}

FReply SDraggablePanel::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	OnFocusGained.ExecuteIfBound();

	return FReply::Handled();
}

void SDraggablePanel::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	OnLostFocus.ExecuteIfBound();
}

void SDraggablePanel::ClosePanel() const
{
	OnPanelClosed.ExecuteIfBound();
}

void SDraggablePanel::SetBorderColor(const FLinearColor InBorderColor) const
{
	TitleBar->SetBorderColor(InBorderColor);
}

void SDraggablePanel::SetBorderBrush(const FSlateBrush& InBorderBrush) const
{
	TitleBar->SetBorderBrush(InBorderBrush);
}

void SDraggablePanel::SetText(const FText& InText) const
{
	TitleBar->SetText(InText);
}

void SDraggablePanel::SetTextColor(const FLinearColor InTextColor) const
{
	TitleBar->SetTextColor(InTextColor);
}

void SDraggablePanel::SetFont(const FSlateFontInfo& InFont) const
{
	TitleBar->SetFont(InFont);
}

void SDraggablePanel::SetTextPadding(const FMargin InTextPadding) const
{
	TitleBar->SetTextPadding(InTextPadding);
}

void SDraggablePanel::SetCloseButtonSize(const FVector2D InCloseButtonSize) const
{
	TitleBar->SetCloseButtonSize(InCloseButtonSize);
}

void SDraggablePanel::SetButtonContentPadding(const FMargin InButtonContentPadding) const
{
	TitleBar->SetButtonContentPadding(InButtonContentPadding);
}

void SDraggablePanel::SetButtonColor(const FLinearColor InButtonColor) const
{
	TitleBar->SetButtonColor(InButtonColor);
}

void SDraggablePanel::SetButtonTextColor(const FLinearColor InButtonTextColor) const
{
	TitleBar->SetButtonTextColor(InButtonTextColor);
}

void SDraggablePanel::SetButtonStyle(const FButtonStyle& InButtonStyle) const
{
	TitleBar->SetButtonStyle(InButtonStyle);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
