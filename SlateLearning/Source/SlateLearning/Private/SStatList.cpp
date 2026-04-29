// Fill out your copyright notice in the Description page of Project Settings.


#include "SStatList.h"

#include "SlateOptMacros.h"
#include "SlateLearning/Structs/FStatRow.h"
#include "FSlatePracticeStyle.h"
#include "Widgets/Notifications/SProgressBar.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SStatList::Construct(const FArguments& InArgs)
{
	StatRows = InArgs._StatRows;
	CachedRows = StatRows.Get();
	
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(300.f)
		.HeightOverride(200.f)
		[
			SAssignNew(ListView, SListView<TSharedPtr<FStatRow>>)
			.ListItemsSource(&CachedRows)
			.OnGenerateRow(this, &SStatList::GenerateRow)
			.HeaderRow(
				SNew(SHeaderRow)
				+ SHeaderRow::Column("Name")
				.DefaultLabel(INVTEXT("Stat"))
				.OnSort(this, &SStatList::OnSort)
				+ SHeaderRow::Column("Value")
				.DefaultLabel(INVTEXT("Value"))
				.OnSort(this, &SStatList::OnSort)
				)
		]
	];
	
	RegisterActiveTimer(0.1f, FWidgetActiveTimerDelegate::CreateSP(this, &SStatList::RefreshTimer));
}

TSharedRef<ITableRow> SStatList::GenerateRow(TSharedPtr<FStatRow> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FStatRow>>, OwnerTable)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			[
				SNew(STextBlock)
				.Text(Item->StatName)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(Item->StatValue)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8.f, 0.f,0.f,0.f)
			[
				SNew(SBox)
				.WidthOverride(80.f)
				.HeightOverride(8.f)
				[
					SNew(SProgressBar)
					.Percent(Item->NormalizedValue)
					.BarFillType(EProgressBarFillType::LeftToRight)
					.Style(FSlatePracticeStyle::Get(), "SlatePractice.HealthBar")
				]
			]
		];
}

void SStatList::OnSort(EColumnSortPriority::Type SortPriority, const FName& ColumnName, EColumnSortMode::Type SortMode)
{
	if (ColumnName == "Name")
	{
		CachedRows.Sort([](const TSharedPtr<FStatRow>& A, const TSharedPtr<FStatRow>& B)
		{
			return A->StatName.ToString() < B->StatName.ToString();
		});
	} 
	else if (ColumnName == "Value")
	{
		CachedRows.Sort([](const TSharedPtr<FStatRow>& A, const TSharedPtr<FStatRow>& B)
		{
			return A->NormalizedValue > B->NormalizedValue;
		});
	}
	
	ListView->RequestListRefresh();
}

EActiveTimerReturnType SStatList::RefreshTimer(double InCurrentTime, float InDeltaTime)
{
	if (ListView.IsValid())
	{
		CachedRows = StatRows.Get();
		ListView->RequestListRefresh();
	}
	
	return EActiveTimerReturnType::Continue;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
