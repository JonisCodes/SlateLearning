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
	PanelHeight = InArgs._PanelHeight;
	PanelColor = InArgs._PanelColor;
	SearchBoxHeight = InArgs._SearchBoxHeight;
	RowFont = InArgs._RowFont;

	ChildSlot
	[
		SNew(SBox)
		.HeightOverride(PanelHeight)
		[
			SNew(SOverlay)

			// Panel background
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.ColorAndOpacity_Lambda([this]()
				{
					return PanelColor;
				})
				.Image_Lambda([this]()
				{
					return PanelBrush;
				})
			]

			// Search box
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			[
				SAssignNew(SearchBox, SComboSearchBox)
				.OnTextChangedDelegate(this, &SComboPanel::OnTextChanged)
				.SearchBoxHeight(SearchBoxHeight)
			]

			// Scroll box
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(0.f, SearchBoxHeight, 0.f, 0.f)
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
		.Font(RowFont)
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

void SComboPanel::SetPanelHeight(const float InPanelHeight)
{
	PanelHeight = InPanelHeight;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SComboPanel::SetPanelColor(const FLinearColor InPanelColor)
{
	PanelColor = InPanelColor;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SComboPanel::SetSearchBoxHeight(const float InSearchBoxHeight)
{
	SearchBoxHeight = InSearchBoxHeight;
	SearchBox->SetSearchBoxHeight(InSearchBoxHeight);
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SComboPanel::SetRowFont(const FSlateFontInfo InRowFont)
{
	RowFont = InRowFont;
}

void SComboPanel::SetPanelBrush(const FSlateBrush* InPanelBrush)
{
	PanelBrush = InPanelBrush;
	Invalidate(EInvalidateWidgetReason::Paint);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
