// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
DECLARE_DELEGATE(FOnPanelClosed)
DECLARE_DELEGATE_OneParam(FOnPanelMoved, FVector2D)

class SLATELEARNING_API STitlebar : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STitlebar)
			: _BorderColor(FLinearColor::White)
			  , _BorderBrush(*FCoreStyle::Get().GetBrush("WhiteBrush"))
			  , _TextPadding(FMargin(5.f, 2.f))
			  , _Text(INVTEXT("Title Bar"))
			  , _TextColor(FLinearColor::White)
			  , _Font()
			  , _CloseButtonSize(FVector2D(32.f, 32.f))
			  , _ButtonContentPadding(FMargin(0.f, 7.f))
			  , _ButtonColor(FLinearColor::White)
			  , _ButtonTextColor(FLinearColor::Black)
			  , _ButtonStyle(FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button"))
		{
		}

		// Background border
		SLATE_ATTRIBUTE(FLinearColor, BorderColor)
		SLATE_ATTRIBUTE(FSlateBrush, BorderBrush)

		// Left Title Text
		SLATE_ATTRIBUTE(FMargin, TextPadding)
		SLATE_ATTRIBUTE(FText, Text)
		SLATE_ATTRIBUTE(FLinearColor, TextColor)
		SLATE_ATTRIBUTE(FSlateFontInfo, Font)

		// Close Button
		SLATE_ATTRIBUTE(FVector2D, CloseButtonSize)
		SLATE_ATTRIBUTE(FMargin, ButtonContentPadding)
		SLATE_ATTRIBUTE(FLinearColor, ButtonColor)
		SLATE_ATTRIBUTE(FLinearColor, ButtonTextColor)
		SLATE_ATTRIBUTE(FButtonStyle, ButtonStyle)
		SLATE_EVENT(FOnPanelClosed, OnPanelClosed)

		SLATE_EVENT(FOnPanelMoved, OnPanelMove)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	void SetBorderColor(FLinearColor InBorderColor);
	void SetBorderBrush(FSlateBrush InBorderBrush);
	void SetText(FText InText);
	void SetTextColor(FLinearColor InTextColor);
	void SetFont(FSlateFontInfo InFont);
	void SetTextPadding(FMargin InTextPadding);
	void SetCloseButtonSize(FVector2D InCloseButtonSize);
	void SetButtonContentPadding(FMargin InButtonContentPadding);
	void SetButtonColor(FLinearColor InButtonColor);
	void SetButtonTextColor(FLinearColor InButtonTextColor);
	void SetButtonStyle(FButtonStyle InButtonStyle);

protected:
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;

private:
	FOnPanelClosed OnPanelClosed;
	FOnPanelMoved OnPanelMove;

	FReply HandleCloseClicked() const;

	bool bIsDragging = false;

	TAttribute<FLinearColor> BorderColor;
	TAttribute<FSlateBrush> BorderBrush;
	TAttribute<FMargin> TextPadding;
	TAttribute<FText> Text;
	TAttribute<FLinearColor> TextColor;
	TAttribute<FSlateFontInfo> Font;
	TAttribute<FVector2D> CloseButtonSize;
	TAttribute<FMargin> ButtonContentPadding;
	TAttribute<FLinearColor> ButtonColor;
	TAttribute<FLinearColor> ButtonTextColor;
	TAttribute<FButtonStyle> ButtonStyle;

	TSharedPtr<SBorder> TitleBorder;
	TSharedPtr<SBorder> TextPaddingWrapper;
	TSharedPtr<STextBlock> TitleTextBlock;
	TSharedPtr<SBox> CloseButtonSizeBox;
	TSharedPtr<SBorder> ButtonContentWrapper;
	TSharedPtr<SButton> CloseButton;
};
