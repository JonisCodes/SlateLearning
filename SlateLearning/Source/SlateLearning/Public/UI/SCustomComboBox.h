// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomComboBox.h"
#include "FComboBoxItem.h"
#include "Widgets/SCompoundWidget.h"

struct FComboBoxItem;
class SComboHeader;
class SComboPanel;
/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnSelectionChanged, const FComboBoxItem& Item);

class SLATELEARNING_API SCustomComboBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCustomComboBox)
			: _DesiredWidth(300.f)
			  , _HeaderHeight(24.f)
			  , _PanelHeight(300.f)
			  , _SearchBoxHeight(24.f)
			  , _PanelColor(FLinearColor(1.f, 1.f, 1.f, 0.5f))
			  , _HeaderColor(FLinearColor::White)
			  , _HeaderFontColor(FLinearColor::Black)
			  , _HeaderArrowColor(FLinearColor::Black)
			  , _HeaderBrush(FCoreStyle::Get().GetBrush("WhiteBrush"))
			  , _PanelBrush(FCoreStyle::Get().GetBrush("WhiteBrush"))
			  , _HeaderFont(FCoreStyle::Get().GetFontStyle("NormalText"))
			  , _RowFont(FCoreStyle::Get().GetFontStyle("NormalText"))
			  , _bOpenComboBox(false)
		{
		}

		SLATE_ARGUMENT(float, DesiredWidth)
		SLATE_ARGUMENT(float, HeaderHeight)
		SLATE_ARGUMENT(float, PanelHeight)
		SLATE_ARGUMENT(float, SearchBoxHeight)
		SLATE_ARGUMENT(FLinearColor, PanelColor)
		SLATE_ARGUMENT(FLinearColor, HeaderColor)
		SLATE_ARGUMENT(FLinearColor, HeaderFontColor)
		SLATE_ARGUMENT(FLinearColor, HeaderArrowColor)
		SLATE_ARGUMENT(const FSlateBrush*, HeaderBrush)
		SLATE_ARGUMENT(const FSlateBrush*, PanelBrush)
		SLATE_ARGUMENT(FSlateFontInfo, HeaderFont)
		SLATE_ARGUMENT(FSlateFontInfo, RowFont)
		SLATE_ARGUMENT(bool, bOpenComboBox)
		SLATE_EVENT(FOnSelectionChanged, OnSelectionChanged)

	SLATE_END_ARGS()

	void OnApplicationMouseDown(const FPointerEvent& PointerEvent);
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual ~SCustomComboBox() override;
	virtual bool SupportsKeyboardFocus() const override { return true; }
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;


	void OpenComboBox();
	void CloseComboBox();
	void ToggleComboBox();
	void OnRowSelected(const FComboBoxItem& Item);
	void AddItem(const FComboBoxItem& Item) const;

	void SetDesiredWidth(const float InDesiredWidth);
	void SetHeaderHeight(const float InHeaderHeight);
	void SetPanelHeight(const float InPanelHeight);
	void SetSearchBoxHeight(const float InSearchBoxHeight);
	void SetPanelColor(const FLinearColor InPanelColor);
	void SetHeaderColor(const FLinearColor InHeaderColor);
	void SetHeaderFontColor(const FLinearColor InHeaderFontColor);
	void SetHeaderArrowColor(const FLinearColor InHeaderArrowColor);
	void SetHeaderBrush(const FSlateBrush* InHeaderBrush);
	void SetPanelBrush(FSlateBrush* InPanelBrush);
	void SetHeaderFont(const FSlateFontInfo InHeaderFont);
	void SetRowFont(const FSlateFontInfo InRowFont);
	void SetOpenComboBox(bool bOpen);

	FOnSelectionChanged OnSelectionChanged;

private:
	float DesiredWidth = 300.f;
	float HeaderHeight = 24.f;
	bool bIsOpen = false;
	FComboBoxItem CurrentSelectedItem;

	TSharedPtr<SComboPanel> ComboPanel;
	TSharedPtr<SComboHeader> ComboHeader;

	FDelegateHandle MouseDownHandle;

	float SearchBoxHeight = 24.f;
	float PanelHeight = 300.f;
	FLinearColor PanelColor = FLinearColor(1.f, 1.f, 1.f, 0.5f);

	FLinearColor HeaderColor = FLinearColor::White;
	FLinearColor HeaderFontColor = FLinearColor::Black;
	FLinearColor HeaderArrowColor = FLinearColor::Black;
	const FSlateBrush* HeaderBrush = nullptr;
	const FSlateBrush* PanelBrush = nullptr;
	FSlateFontInfo HeaderFont;
	FSlateFontInfo RowFont;
	bool bOpenComboBox = false;
};
