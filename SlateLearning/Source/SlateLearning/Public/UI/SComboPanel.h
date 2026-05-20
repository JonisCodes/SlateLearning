// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
		{
		}

		SLATE_EVENT(FOnRowSelected, OnRowSelected)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void AddElement(const FComboBoxItem& InItem);
	void OnRowClicked(const FComboBoxItem& Item) const;
	void OnTextChanged(const FText& Text) const;
	void OnPanelOpened() const;

private:
	TSharedPtr<SScrollBox> ScrollBox;
	TSharedPtr<SComboSearchBox> SearchBox;
	FOnRowSelected OnRowSelected;

	TArray<TSharedPtr<SCustomComboRow>> ComboRows;
};
