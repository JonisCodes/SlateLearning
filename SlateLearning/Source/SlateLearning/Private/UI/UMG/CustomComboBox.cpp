// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/CustomComboBox.h"

#include "IContentBrowserSingleton.h"
#include "SCustomComboBox.h"

void UCustomComboBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (CustomComboBox.IsValid())
	{
		CustomComboBox->SetDesiredWidth(DesiredWidth);
		CustomComboBox->SetHeaderHeight(HeaderHeight);
		CustomComboBox->SetPanelHeight(PanelHeight);
		CustomComboBox->SetSearchBoxHeight(SearchBoxHeight);
		CustomComboBox->SetPanelColor(PanelColor);
		CustomComboBox->SetHeaderColor(HeaderColor);
		CustomComboBox->SetHeaderFontColor(HeaderFontColor);
		CustomComboBox->SetHeaderArrowColor(HeaderArrowColor);
		CustomComboBox->SetHeaderBrush(&HeaderBrush);
		CustomComboBox->SetPanelBrush(&PanelBrush);
		CustomComboBox->SetHeaderFont(HeaderFont);
		CustomComboBox->SetRowFont(RowFont);
		CustomComboBox->SetOpenComboBox(bOpenComboBox);
	}
}

bool UCustomComboBox::AddItem(const FLinearColor BackgroundColor, const FText DisplayName, const FLinearColor TextColor)
{
	if (!CustomComboBox.IsValid()) return false;

	FComboBoxItem Item;
	Item.BackgroundColor = BackgroundColor;
	Item.TextColor = TextColor;
	Item.DisplayName = DisplayName;

	CustomComboBox->AddItem(Item);
	return true;
}

TSharedRef<SWidget> UCustomComboBox::RebuildWidget()
{
	return SAssignNew(CustomComboBox, SCustomComboBox)
		.SearchBoxHeight(SearchBoxHeight)
		.PanelHeight(PanelHeight)
		.DesiredWidth(DesiredWidth)
		.HeaderHeight(HeaderHeight)
		.PanelColor(PanelColor)
		.HeaderColor(HeaderColor)
		.HeaderFontColor(HeaderFontColor)
		.HeaderArrowColor(HeaderArrowColor)
		.HeaderBrush(&HeaderBrush)
		.PanelBrush(&PanelBrush)
		.HeaderFont(HeaderFont)
		.RowFont(RowFont)
		.bOpenComboBox(bOpenComboBox)
		.OnSelectionChanged(BIND_UOBJECT_DELEGATE(FOnSelectionChanged, HandleSelectionChanged));
}

void UCustomComboBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	CustomComboBox.Reset();
}

void UCustomComboBox::HandleSelectionChanged(const FComboBoxItem& SelectedItem)
{
	OnSelectionChanged.Broadcast(SelectedItem);
}
