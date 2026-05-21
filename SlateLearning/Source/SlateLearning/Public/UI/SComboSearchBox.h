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
			: _SearchBoxHeight(24.f)
		{
		}

		SLATE_EVENT(FOnTextChanged, OnTextChangedDelegate)
		SLATE_ARGUMENT(float, SearchBoxHeight)
	SLATE_END_ARGS()

	void OnTextChanged(const FText& Text) const;
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	TSharedPtr<SEditableTextBox> GetSearchBox() const { return SearchBox; }

	float GetSearchBoxHeight() const { return SearchBoxHeight; }

	void SetSearchBoxHeight(const float InSearchBoxHeight);

private:
	TSharedPtr<SEditableTextBox> SearchBox;
	FOnTextChanged OnTextChangedDelegate;

	float SearchBoxHeight = 24.f;
};
