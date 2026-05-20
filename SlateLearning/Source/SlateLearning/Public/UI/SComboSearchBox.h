// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnTextChanged, const FText&);

class SLATELEARNING_API SComboSearchBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SComboSearchBox)
		{
		}

		SLATE_EVENT(FOnTextChanged, OnTextChangedDelegate)
	SLATE_END_ARGS()

	void OnTextChanged(const FText& Text) const;
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	TSharedPtr<SEditableTextBox> GetSearchBox() const { return SearchBox; }

private:
	TSharedPtr<SEditableTextBox> SearchBox;
	FOnTextChanged OnTextChangedDelegate;
};
