// Fill out your copyright notice in the Description page of Project Settings.


#include "SCustomComboBox.h"

#include "FComboBoxItem.h"
#include "SComboHeader.h"
#include "SComboPanel.h"
#include "SEditorViewportToolBarMenu.h"
#include "SlateOptMacros.h"
#include "Widgets/SCanvas.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SCustomComboBox::OnApplicationMouseDown(const FPointerEvent& PointerEvent)
{
	if (!bIsOpen) return;

	const FGeometry& MyGeometry = GetCachedGeometry();

	if (const bool bClickedInside = MyGeometry.IsUnderLocation(PointerEvent.GetScreenSpacePosition());
		!bClickedInside)
	{
		CloseComboBox();
	}
}

void SCustomComboBox::Construct(const FArguments& InArgs)
{
	DesiredWidth = InArgs._DesiredWidth;
	HeaderHeight = InArgs._HeaderHeight;
	SearchBoxHeight = InArgs._SearchBoxHeight;
	PanelHeight = InArgs._PanelHeight;
	PanelColor = InArgs._PanelColor;
	HeaderColor = InArgs._HeaderColor;
	HeaderFontColor = InArgs._HeaderFontColor;
	HeaderArrowColor = InArgs._HeaderArrowColor;
	HeaderBrush = InArgs._HeaderBrush;
	bOpenComboBox = InArgs._bOpenComboBox;

	OnSelectionChanged = InArgs._OnSelectionChanged;

	ChildSlot
	[
		SNew(SCanvas)

		// header first = lower Z
		+ SCanvas::Slot()
		.Position(FVector2D(0, 0))
		.Size_Lambda([this]()
		{
			return FVector2D(DesiredWidth, HeaderHeight);
		})
		[
			SAssignNew(ComboHeader, SComboHeader)
			.DesiredWidth(DesiredWidth)
			.BackgroundColor(HeaderColor)
			.OnHeaderClicked(this, &SCustomComboBox::ToggleComboBox)
			.FontColor(HeaderFontColor)
			.ArrowColor(HeaderArrowColor)
			.HeaderFont(HeaderFont)
		]

		// panel second = higher Z, floats on top
		+ SCanvas::Slot()
		.Position_Lambda([this]()
		{
			return FVector2D(0, HeaderHeight);
		})
		.Size_Lambda([this]()
		{
			return FVector2D(DesiredWidth, PanelHeight);
		})
		[
			SAssignNew(ComboPanel, SComboPanel)
			.Visibility_Lambda([this]()
			{
				return bOpenComboBox ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed;
			})
			.OnRowSelected(this, &SCustomComboBox::OnRowSelected)
			.SearchBoxHeight(SearchBoxHeight)
			.PanelColor(PanelColor)
			.PanelHeight(PanelHeight)
		]
	];
}

SCustomComboBox::~SCustomComboBox()
{
	if (FSlateApplication::Get().IsInitialized() && MouseDownHandle.IsValid())
	{
		FSlateApplication::Get().OnApplicationMousePreInputButtonDownListener().Remove(MouseDownHandle);
	}
}

FReply SCustomComboBox::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Unhandled();
}

FVector2D SCustomComboBox::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D(DesiredWidth, HeaderHeight);
}

void SCustomComboBox::OpenComboBox()
{
	if (!ComboPanel.IsValid()) return;

	if (bIsOpen) return;

	ComboPanel->SetVisibility(EVisibility::SelfHitTestInvisible);
	bIsOpen = true;
}

void SCustomComboBox::CloseComboBox()
{
	if (!ComboPanel.IsValid()) return;

	if (!bIsOpen) return;

	ComboPanel->SetVisibility(EVisibility::Collapsed);
	bIsOpen = false;
	FSlateApplication::Get().OnApplicationMousePreInputButtonDownListener().Remove(MouseDownHandle);
	MouseDownHandle.Reset();
}

void SCustomComboBox::ToggleComboBox()
{
	bIsOpen = !bIsOpen;
	UE_LOG(LogTemp, Warning, TEXT("ToggleOpen: %s"), bIsOpen ? TEXT("Open") : TEXT("Closed"))
	ComboPanel->SetVisibility(bIsOpen ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed);
	if (bIsOpen)
	{
		MouseDownHandle = FSlateApplication::Get().OnApplicationMousePreInputButtonDownListener().AddSP(
			this, &SCustomComboBox::OnApplicationMouseDown);
		ComboPanel->OnPanelOpened();
	}
	else
	{
		FSlateApplication::Get().OnApplicationMousePreInputButtonDownListener().Remove(MouseDownHandle);
		MouseDownHandle.Reset();
	}
	Invalidate(EInvalidateWidgetReason::Layout);
}

void SCustomComboBox::OnRowSelected(const FComboBoxItem& Item)
{
	CurrentSelectedItem = Item;
	ComboHeader->SetSelectedItem(Item);
	OnSelectionChanged.ExecuteIfBound(CurrentSelectedItem);
	CloseComboBox();
}

void SCustomComboBox::AddItem(const FComboBoxItem& Item) const
{
	if (!ComboPanel.IsValid()) return;
	ComboPanel->AddElement(Item);
}

void SCustomComboBox::SetDesiredWidth(const float InDesiredWidth)
{
	DesiredWidth = InDesiredWidth;
}

void SCustomComboBox::SetHeaderHeight(const float InHeaderHeight)
{
	HeaderHeight = InHeaderHeight;
}

void SCustomComboBox::SetPanelHeight(const float InPanelHeight)
{
	if (!ComboPanel.IsValid()) return;

	PanelHeight = InPanelHeight;
	ComboPanel->SetPanelHeight(InPanelHeight);
}

void SCustomComboBox::SetSearchBoxHeight(const float InSearchBoxHeight)
{
	if (!ComboPanel.IsValid()) return;

	SearchBoxHeight = InSearchBoxHeight;
	ComboPanel->SetSearchBoxHeight(InSearchBoxHeight);
}

void SCustomComboBox::SetPanelColor(const FLinearColor InPanelColor)
{
	if (!ComboPanel.IsValid()) return;

	PanelColor = InPanelColor;
	ComboPanel->SetPanelColor(PanelColor);
}

void SCustomComboBox::SetHeaderColor(const FLinearColor InHeaderColor)
{
	if (!ComboHeader.IsValid()) return;

	HeaderColor = InHeaderColor;
	ComboHeader->SetHeaderColor(HeaderColor);
}

void SCustomComboBox::SetHeaderFontColor(const FLinearColor InHeaderFontColor)
{
	if (!ComboHeader.IsValid()) return;

	HeaderFontColor = InHeaderFontColor;
	ComboHeader->SetFontColor(HeaderFontColor);
}

void SCustomComboBox::SetHeaderArrowColor(const FLinearColor InHeaderArrowColor)
{
	if (!ComboHeader.IsValid()) return;

	HeaderArrowColor = InHeaderArrowColor;
	ComboHeader->SetArrowColor(HeaderArrowColor);
}

void SCustomComboBox::SetHeaderBrush(const FSlateBrush* InHeaderBrush)
{
	if (!ComboHeader.IsValid()) return;

	HeaderBrush = InHeaderBrush;
	ComboHeader->SetHeaderBrush(HeaderBrush);
}

void SCustomComboBox::SetPanelBrush(FSlateBrush* InPanelBrush)
{
	if (!ComboPanel.IsValid()) return;
	PanelBrush = InPanelBrush;
	ComboPanel->SetPanelBrush(PanelBrush);
}

void SCustomComboBox::SetHeaderFont(const FSlateFontInfo InHeaderFont)
{
	if (!ComboHeader.IsValid()) return;

	HeaderFont = InHeaderFont;
	ComboHeader->SetHeaderFont(HeaderFont);
}

void SCustomComboBox::SetRowFont(const FSlateFontInfo InRowFont)
{
	if (!ComboPanel.IsValid()) return;

	RowFont = InRowFont;
	ComboPanel->SetRowFont(RowFont);
}

void SCustomComboBox::SetOpenComboBox(const bool bOpen)
{
	bOpenComboBox = bOpen;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
