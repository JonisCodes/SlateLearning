// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCustomComboBox.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SScrollBox.h"

class SComboSearchBox;
struct FComboBoxItem;
class SCustomComboRow;
/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnRowSelected, const FComboBoxItem&);

class SLATELEARNING_API SComboPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SComboPanel)
			: _PanelHeight(300.f)
			  , _PanelColor(FLinearColor(1.f, 1.f, 1.f, 0.5f))
			  , _SearchBoxHeight(24.f)
			  , _RowFont(FCoreStyle::Get().GetFontStyle("NormalText"))
			  , _PanelBrush(FCoreStyle::Get().GetBrush("WhiteBox"))
		{
		}

		SLATE_EVENT(FOnRowSelected, OnRowSelected)
		SLATE_ARGUMENT(float, PanelHeight)
		SLATE_ARGUMENT(FLinearColor, PanelColor)
		SLATE_ARGUMENT(float, SearchBoxHeight)
		SLATE_ARGUMENT(FSlateFontInfo, RowFont)
		SLATE_ARGUMENT(const FSlateBrush*, PanelBrush)
	SLATE_END_ARGS()


	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void AddElement(const FComboBoxItem& InItem);
	void OnRowClicked(const FComboBoxItem& Item) const;
	void OnTextChanged(const FText& Text) const;
	void OnPanelOpened() const;

	// Setters
	void SetPanelHeight(const float InPanelHeight);
	void SetPanelColor(const FLinearColor InPanelColor);
	void SetSearchBoxHeight(const float InSearchBoxHeight);
	void SetRowFont(const FSlateFontInfo InRowFont);
	void SetPanelBrush(const FSlateBrush* InPanelBrush);

private:
	TSharedPtr<SScrollBox> ScrollBox;
	TSharedPtr<SComboSearchBox> SearchBox;
	FOnRowSelected OnRowSelected;

	TArray<TSharedPtr<SCustomComboRow>> ComboRows;

	float PanelHeight = 300.f;
	float SearchBoxHeight = 24.f;
	FLinearColor PanelColor = FLinearColor(1.f, 1.f, 1.f, 0.5f);
	FSlateFontInfo RowFont;
	const FSlateBrush* PanelBrush = FCoreStyle::Get().GetBrush("WhiteBox");
};
