// Fill out your copyright notice in the Description page of Project Settings.

#include "MySlatePlayerController.h"

#include "MySlateCharacter.h"
#include "SMyHUD.h"
#include "Engine/GameViewportClient.h"
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
