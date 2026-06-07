// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class STitlebar;
/**
 * 
 */
DECLARE_DELEGATE(FOnFocusGained)
DECLARE_DELEGATE(FOnLostFocus)
DECLARE_DELEGATE_OneParam(FOnDragged, FVector2D)
DECLARE_DELEGATE(FOnPanelClosed)

class SLATELEARNING_API SDraggablePanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDraggablePanel)
			: _TitleBorderColor(FLinearColor::White)
			  , _TitleBorderBrush(*FCoreStyle::Get().GetBrush("WhiteBrush"))
			  , _TitleTextPadding(FMargin(5.f, 2.f))
			  , _TitleText(INVTEXT("Title Bar"))
			  , _TitleTextColor(FLinearColor::White)
			  , _TitleFont()
			  , _TitleCloseButtonSize(FVector2D(32.f, 32.f))
			  , _TitleButtonContentPadding(FMargin(0.f, 7.f))
			  , _TitleButtonColor(FLinearColor::White)
			  , _TitleButtonTextColor(FLinearColor::Black)
			  , _TitleCloseButtonStyle()
		{
		}

		// TITLE BAR

		// Background border
		SLATE_ATTRIBUTE(FLinearColor, TitleBorderColor)
		SLATE_ATTRIBUTE(FSlateBrush, TitleBorderBrush)

		// Left Title Text
		SLATE_ATTRIBUTE(FMargin, TitleTextPadding)
		SLATE_ATTRIBUTE(FText, TitleText)
		SLATE_ATTRIBUTE(FLinearColor, TitleTextColor)
		SLATE_ATTRIBUTE(FSlateFontInfo, TitleFont)

		// Close Button
		SLATE_ATTRIBUTE(FVector2D, TitleCloseButtonSize)
		SLATE_ATTRIBUTE(FMargin, TitleButtonContentPadding)
		SLATE_ATTRIBUTE(FLinearColor, TitleButtonColor)
		SLATE_ATTRIBUTE(FLinearColor, TitleButtonTextColor)
		SLATE_ATTRIBUTE(FButtonStyle, TitleCloseButtonStyle)

		SLATE_EVENT(FOnFocusGained, OnFocusGained)
		SLATE_EVENT(FOnLostFocus, OnLostFocus)
		SLATE_EVENT(FOnDragged, OnDragged)
		SLATE_EVENT(FOnPanelClosed, OnPanelClosed)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	void SetContent(TSharedRef<SWidget> InContent);
	void OnTitleBarDragged(FVector2D Delta);

	virtual bool SupportsKeyboardFocus() const override { return true; }
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;

	void ClosePanel() const;


	void SetBorderColor(FLinearColor InBorderColor) const;
	void SetBorderBrush(const FSlateBrush& InBorderBrush) const;
	void SetText(const FText& InText) const;
	void SetTextColor(FLinearColor InTextColor) const;
	void SetFont(const FSlateFontInfo& InFont) const;
	void SetTextPadding(FMargin InTextPadding) const;
	void SetCloseButtonSize(FVector2D InCloseButtonSize) const;
	void SetButtonContentPadding(FMargin InButtonContentPadding) const;
	void SetButtonColor(FLinearColor InButtonColor) const;
	void SetButtonTextColor(FLinearColor InButtonTextColor) const;
	void SetButtonStyle(const FButtonStyle& InButtonStyle) const;

private:
	TSharedPtr<SBox> ContentArea;
	TSharedPtr<STitlebar> TitleBar;
	FVector2D PanelPosition = FVector2D::ZeroVector;

	FOnFocusGained OnFocusGained;
	FOnLostFocus OnLostFocus;
	FOnDragged OnDragged;
	FOnPanelClosed OnPanelClosed;

	TAttribute<FLinearColor> TitleBorderColor;
	TAttribute<FSlateBrush> TitleBorderBrush;
	TAttribute<FMargin> TitleTextPadding;
	TAttribute<FText> TitleText;
	TAttribute<FLinearColor> TitleTextColor;
	TAttribute<FSlateFontInfo> TitleFont;
	TAttribute<FVector2D> TitleCloseButtonSize;
	TAttribute<FMargin> TitleButtonContentPadding;
	TAttribute<FLinearColor> TitleButtonColor;
	TAttribute<FLinearColor> TitleButtonTextColor;
	TAttribute<FButtonStyle> TitleCloseButtonStyle;
};
