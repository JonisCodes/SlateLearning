// Fill out your copyright notice in the Description page of Project Settings.

#include "MySlatePlayerController.h"

#include "MySlateCharacter.h"
#include "SMyHUD.h"
#include "Engine/GameViewportClient.h"
#include "Inventory/SInventoryGrid.h"
#include "UI/SDialogueWidget.h"

void AMySlatePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (const auto MyCharacter = Cast<AMySlateCharacter>(GetPawn()); GEngine && GEngine->GameViewport && MyCharacter)
	{
		HUDWidget = SNew(SMyHUD)
			.OwningCharacter(MyCharacter);
		
		GEngine->GameViewport->AddViewportWidgetContent(
			HUDWidget.ToSharedRef(), 5);
		
		MyCharacter->OnHealthChanged.AddLambda(
			[this](const float Health, const float MaxHealth)
			{
				if (HUDWidget.IsValid())
					HUDWidget->OnHealthChanged(Health, MaxHealth);
			});
		
		MyCharacter->OnAmmoChanged.AddLambda(
			[this](const int32 Ammo, const int32 MaxAmmo)
			{
				if (HUDWidget.IsValid())
					HUDWidget->OnAmmoChanged(Ammo, MaxAmmo);
			});
		
		MyCharacter->OnArmorChanged.AddLambda(
			[this](const float Armor, const float MaxArmor)
			{
				if (HUDWidget.IsValid())
					HUDWidget->OnArmorChanged(Armor, MaxArmor);
			});
		
		TSharedPtr<SInventoryGrid> InventoryGrid;
		SAssignNew(InventoryGrid, SInventoryGrid)
			.Columns(5)
			.Rows(4);
		
		FInventoryItem Sword;
		Sword.Name = FText::FromString("Iron Sword");
		Sword.Description = FText::FromString("A basic sword.");
		Sword.Type = EItemType::Weapon;
		Sword.Color = FLinearColor(0.2f, 0.4f, 1.f);
		InventoryGrid->SetItemAtSlot(0, Sword);
		
		FInventoryItem HealthPotion;
		HealthPotion.Name = FText::FromString("Health Potion");
		HealthPotion.Description = FText::FromString("A healing Potion.");
		HealthPotion.Type = EItemType::Consumable;
		HealthPotion.Color = FLinearColor(1.f, 0.f, 0.f);
		InventoryGrid->SetItemAtSlot(1, HealthPotion);
		InventoryGrid->SetItemAtSlot(3, HealthPotion);
		InventoryGrid->SetItemAtSlot(4, HealthPotion);
		
		GEngine->GameViewport->AddViewportWidgetContent(InventoryGrid.ToSharedRef(), 30);
	}

}

void AMySlatePlayerController::ShowDialogueWidget(UNarrativeComponent* NarrativeComp)
{
	if (!DialogueWidget && DialogueWidgetClass)
	{
		DialogueWidget = CreateWidget<UDialogueWidget>(
			this, DialogueWidgetClass);
	}

	if (DialogueWidget)
	{
		DialogueWidget->InitializeWithComponent(NarrativeComp);
		DialogueWidget->AddToViewport(10);
		DialogueWidget->ShowDialogue();
	}

	OnNarrativeStarted();
}

void AMySlatePlayerController::OnNarrativeStarted()
{
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	SetShowMouseCursor(true);
}

void AMySlatePlayerController::OnNarrativeFinished()
{
	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);
}
