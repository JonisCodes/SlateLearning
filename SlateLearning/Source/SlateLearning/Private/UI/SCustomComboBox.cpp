// Fill out your copyright notice in the Description page of Project Settings.


#include "SCustomComboBox.h"

#include "FComboBoxItem.h"
#include "SComboHeader.h"
#include "SComboPanel.h"
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


	ChildSlot
	[
		SNew(SCanvas)

		// header first = lower Z
		+ SCanvas::Slot()
		.Position(FVector2D(0, 0))
		.Size(FVector2D(DesiredWidth, HeaderHeight))
		[
			SAssignNew(ComboHeader, SComboHeader)
			.DesiredWidth(DesiredWidth)
			.BackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.9f))
			.OnHeaderClicked(this, &SCustomComboBox::ToggleComboBox)
		]

		// panel second = higher Z, floats on top
		+ SCanvas::Slot()
		.Position(FVector2D(0, HeaderHeight))
		.Size(FVector2D(DesiredWidth, 476.f))
		[
			SAssignNew(ComboPanel, SComboPanel)
			.Visibility(EVisibility::Collapsed)
			.OnRowSelected(this, &SCustomComboBox::OnRowSelected)
		]
	];

	FComboBoxItem ElementOne;
	ElementOne.DisplayName = FText(INVTEXT("Test One"));
	AddItem(ElementOne);

	FComboBoxItem ElementTwo;
	ElementTwo.DisplayName = FText(INVTEXT("Test Two"));
	AddItem(ElementTwo);

	FComboBoxItem ElementThree;
	ElementThree.DisplayName = FText(INVTEXT("Test Three"));
	AddItem(ElementThree);

	// AddItem
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
	// if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	// {
	// 	const FVector2D LocalPos = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	// 	const float Height = ComboHeader->GetCachedGeometry().GetLocalSize().Y;
	//
	// 	if (LocalPos.Y <= Height)
	// 	{
	// 		ToggleComboBox();
	// 		return FReply::Handled();
	// 	}
	// }

	return FReply::Unhandled();
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
	ComboPanel->SetVisibility(bIsOpen ? EVisibility::Visible : EVisibility::Collapsed);
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
	ToggleComboBox();
}

void SCustomComboBox::AddItem(const FComboBoxItem& Item) const
{
	if (!ComboPanel.IsValid()) return;
	ComboPanel->AddElement(Item);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
