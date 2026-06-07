// Fill out your copyright notice in the Description page of Project Settings.


#include "RadialMenu.h"

#include "SRadialMenu.h"
#include "Delegates/Delegate.h"

void URadialMenu::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (MyRadialMenu.IsValid())
	{
		MyRadialMenu->SetOuterRadius(MenuOuterDiameter / 2.f);
		MyRadialMenu->SetInnerRadius(MenuInnerDiameter / 2.f);
		MyRadialMenu->SetNumSegments(NumSegments);
	}
}

void URadialMenu::AddItem(const FLinearColor Color, const FLinearColor HoveredColor, const FText Label,
                          const bool bIsValid,
                          const FSlateBrush Brush) const
{
	if (!MyRadialMenu.IsValid()) return;
	FRadialMenuItem NewItem;
	NewItem.Color = Color;
	NewItem.HoveredColor = HoveredColor;
	NewItem.Label = Label;
	NewItem.bIsvalid = bIsValid;
	NewItem.Brush = Brush;

	MyRadialMenu->AddItem(NewItem);
}

void URadialMenu::OpenMenu() const
{
	if (!MyRadialMenu.IsValid()) return;
	UE_LOG(LogTemp, Warning, TEXT("OPEN MENU"));

	MyRadialMenu->SetVisibility(EVisibility::Visible);
}

void URadialMenu::CloseMenu() const
{
	if (!MyRadialMenu.IsValid()) return;

	MyRadialMenu->SetVisibility(EVisibility::Collapsed);
}

void URadialMenu::SetFocusToMenu() const
{
	if (!MyRadialMenu.IsValid()) return;

	if (APlayerController* PC = GetOwningPlayer())
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(MyRadialMenu.ToSharedRef());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
	}

	FSlateApplication::Get().SetUserFocus(0, MyRadialMenu.ToSharedRef(), EFocusCause::SetDirectly);
}

void URadialMenu::SetSelectionKey(const FKey& NewSelectionKey)
{
	if (!MyRadialMenu.IsValid()) return;

	SelectionKey = NewSelectionKey;
	MyRadialMenu->SetSelectionKey(SelectionKey);
}

void URadialMenu::ConfirmSelection() const
{
	MyRadialMenu->ConfirmSelection();
}

TSharedRef<SWidget> URadialMenu::RebuildWidget()
{
	return SNew(SBox)
		.WidthOverride_Lambda([this]()
		{
			return MenuOuterDiameter;
		})
		.HeightOverride_Lambda([this]()
		{
			return MenuOuterDiameter;
		})
		[
			SAssignNew(MyRadialMenu, SRadialMenu)
			.InnerRadius(MenuInnerDiameter / 2.f)
			.OuterRadius(MenuOuterDiameter / 2.f)
			.NumSegments(NumSegments)
			.SelectionKey(SelectionKey)
			.OnSegmentSelected(BIND_UOBJECT_DELEGATE(FOnSegmentSelected, HandleSegmentSelected))
			.OnSegmentHovered(BIND_UOBJECT_DELEGATE(FOnSegmentHovered, HandleSegmentHovered))
			.OnSegmentUnhovered(BIND_UOBJECT_DELEGATE(FOnSegmentUnhovered, HandleSegmentUnhovered))
		];
}

void URadialMenu::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyRadialMenu.Reset();
}

void URadialMenu::HandleSegmentSelected(const FRadialMenuItem& SelectedItem) const
{
	OnSelect.Broadcast(SelectedItem);
	CloseMenu();
}

void URadialMenu::HandleSegmentHovered(const FRadialMenuItem& HoveredItem) const
{
	OnHovered.Broadcast(HoveredItem);
}

void URadialMenu::HandleSegmentUnhovered(const FRadialMenuItem& UnhoveredItem) const
{
	OnUnhovered.Broadcast(UnhoveredItem);
}
