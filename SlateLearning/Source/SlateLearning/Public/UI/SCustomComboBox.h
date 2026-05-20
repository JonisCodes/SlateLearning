// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FComboBoxItem.h"
#include "Widgets/SCompoundWidget.h"

struct FComboBoxItem;
class SComboHeader;
class SComboPanel;
/**
 * 
 */

class SLATELEARNING_API SCustomComboBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCustomComboBox)
			: _DesiredWidth(300.f)
			  , _HeaderHeight(24.f)

		{
		}

		SLATE_ARGUMENT(float, DesiredWidth)
		SLATE_ARGUMENT(float, HeaderHeight)
	SLATE_END_ARGS()

	void OnApplicationMouseDown(const FPointerEvent& PointerEvent);
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	~SCustomComboBox();
	virtual bool SupportsKeyboardFocus() const override { return true; }
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	void OpenComboBox();
	void CloseComboBox();
	void ToggleComboBox();
	void OnRowSelected(const FComboBoxItem& Item);
	void AddItem(const FComboBoxItem& Item) const;

private:
	float DesiredWidth;
	float HeaderHeight;
	bool bIsOpen = false;
	FComboBoxItem CurrentSelectedItem;

	TSharedPtr<SComboPanel> ComboPanel;
	TSharedPtr<SComboHeader> ComboHeader;

	FDelegateHandle MouseDownHandle;
};
