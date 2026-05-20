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

	ChildSlot
	[
		SNew(SBox)
		.HeightOverride(24.f)
		[
			SAssignNew(SearchBox, SEditableTextBox)
			.HintText(FText::FromString("Search Text"))
			.OnTextChanged(this, &SComboSearchBox::OnTextChanged)
		]
	];
}

/*
 *	Call on text changed and bubble it up to the panel with the text but to lower case so it becomes case insensitive
 */
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
