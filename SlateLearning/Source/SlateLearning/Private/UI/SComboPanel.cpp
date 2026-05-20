// Fill out your copyright notice in the Description page of Project Settings.


#include "SComboPanel.h"

#include "FComboBoxItem.h"
#include "SComboSearchBox.h"
#include "SCustomComboRow.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SScrollBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SComboPanel::Construct(const FArguments& InArgs)
{
	OnRowSelected = InArgs._OnRowSelected;

	ChildSlot
	[
		SNew(SBox)
		.HeightOverride(476.f)
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.ColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.5f))
				.Image(FCoreStyle::Get().GetBrush("WhiteBrush"))
			]

			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			[
				SAssignNew(SearchBox, SComboSearchBox)
				.OnTextChangedDelegate(this, &SComboPanel::OnTextChanged)
			]

			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(0.f, 24.f, 0.f, 0.f)
			[
				SAssignNew(ScrollBox, SScrollBox)
			]
		]
	];
}

void SComboPanel::AddElement(const FComboBoxItem& InItem)
{
	if (!ScrollBox.IsValid()) return;

	TSharedPtr<SCustomComboRow> Row;

	ScrollBox->AddSlot()
	[
		SAssignNew(Row, SCustomComboRow)
		.Item(InItem)
		.OnRowClicked(this, &SComboPanel::OnRowClicked)
	];

	ComboRows.Add(Row);
}

void SComboPanel::OnRowClicked(const FComboBoxItem& Item) const
{
	OnRowSelected.ExecuteIfBound(Item);
}

void SComboPanel::OnTextChanged(const FText& Text) const
{
	if (!ScrollBox.IsValid()) return;

	UE_LOG(LogTemp, Warning, TEXT("Text Changed: %s"), *Text.ToString());

	// for (auto Element : ScrollBox->GetAllChildren())
	// {
	// 	if (Text.IsEmpty())
	// 	{
	// 		Element->SetVisibility(EVisibility::Visible);
	// 	}
	//
	// 	const FString TextString = Text.ToString();
	// 	const FString ElementName = Element->Item.DisplayName.ToString();
	// 	Element->SetVisibility(ElementName.Contains(TextString) ? EVisibility::Visible : EVisibility::Collapsed);
	// }
	for (const TSharedPtr<SCustomComboRow>& Row : ComboRows)
	{
		const FString ItemName = Row->Item.DisplayName.ToString();
		const bool bMatches = Text.IsEmpty() || ItemName.Contains(Text.ToString(), ESearchCase::IgnoreCase);
		Row->SetVisibility(bMatches ? EVisibility::Visible : EVisibility::Collapsed);
	}
}

void SComboPanel::OnPanelOpened() const
{
	if (!SearchBox.IsValid()) return;
	FSlateApplication::Get().SetKeyboardFocus(SearchBox->GetSearchBox(), EFocusCause::SetDirectly);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
