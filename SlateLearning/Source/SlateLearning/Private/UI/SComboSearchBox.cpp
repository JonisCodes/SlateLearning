// Fill out your copyright notice in the Description page of Project Settings.


#include "SComboSearchBox.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SComboSearchBox::OnTextChanged(const FText& Text) const
{
	OnTextChangedDelegate.ExecuteIfBound(Text.ToLower());
}

void SComboSearchBox::Construct(const FArguments& InArgs)
{
	OnTextChangedDelegate = InArgs._OnTextChangedDelegate;
	SearchBoxHeight = InArgs._SearchBoxHeight;

	ChildSlot
	[
		SNew(SBox)
		.HeightOverride_Lambda([this]()
		{
			return SearchBoxHeight;
		})
		[
			SAssignNew(SearchBox, SEditableTextBox)
			.HintText(FText::FromString("Search Text"))
			.OnTextChanged(this, &SComboSearchBox::OnTextChanged)
		]
	];
}

void SComboSearchBox::SetSearchBoxHeight(const float InSearchBoxHeight)
{
	SearchBoxHeight = InSearchBoxHeight;
}

/*
 *	Call on text changed and bubble it up to the panel with the text but to lower case so it becomes case insensitive
 */
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
