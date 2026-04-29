// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class AMySlateCharacter;
struct FStatRow;
class SStatList;
/**
 * 
 */
class SLATELEARNINGEDITOR_API SStatInspectorPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SStatInspectorPanel)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual ~SStatInspectorPanel() override;
	
	TSharedRef<SBox> BuildToolbar();
	TSharedRef<SBox> BuildEmptyState();
	
	TSharedPtr<SWidgetSwitcher> ContentSwitcher;
	TSharedPtr<SStatList> StatList;
	TSharedPtr<IDetailsView> DetailsView;
	
private:
	void OnEditorSelectionChanged(UObject* NewSelection);
	FDelegateHandle SelectionChangedHandle;
	TWeakObjectPtr<AMySlateCharacter> SelectedCharacter;
	
	TSharedRef<SWidget> BuildDetailsView();
	TArray<TSharedPtr<FStatRow>> GetStatRows() const;
	void RefreshPanel();
};
