// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FStatRow;
/**
 * 
 */
class SLATELEARNING_API SStatList : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SStatList)
		{
		}
		SLATE_ATTRIBUTE(TArray<TSharedPtr<FStatRow>>, StatRows)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
private:
	TSharedRef<ITableRow> GenerateRow(TSharedPtr<FStatRow> Item,
		const TSharedRef<STableViewBase>& OwnerTable);
	void OnSort(EColumnSortPriority::Type SortPriority, const FName& ColumnName, EColumnSortMode::Type SortMode);
	
	EActiveTimerReturnType RefreshTimer(double InCurrentTime, float InDeltaTime);
	
	TAttribute<TArray<TSharedPtr<FStatRow>>> StatRows;
	TSharedPtr<SListView<TSharedPtr<FStatRow>>> ListView;
	TArray<TSharedPtr<FStatRow>> CachedRows;
};
