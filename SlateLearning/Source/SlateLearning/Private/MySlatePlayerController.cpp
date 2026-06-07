#include "MySlatePlayerController.h"

#include "MySlateCharacter.h"
#include "RadialMenu.h"
#include "RadialMenuWidget.h"
#include "SMyHUD.h"
#include "Engine/GameViewportClient.h"
#include "UI/SDialogueWidget.h"

void AMySlatePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	OnNarrativeStarted();

	// if (const auto MyCharacter = Cast<AMySlateCharacter>(GetPawn()); GEngine && GEngine->GameViewport && MyCharacter)
	// {
	// 	HUDWidget = SNew(SMyHUD)
	// 		.OwningCharacter(MyCharacter);
	//
	// 	GEngine->GameViewport->AddViewportWidgetContent(
	// 		HUDWidget.ToSharedRef(), 5);
	//
	// 	MyCharacter->OnHealthChanged.AddLambda(
	// 		[this](const float Health, const float MaxHealth)
	// 		{
	// 			if (HUDWidget.IsValid())
	// 				HUDWidget->OnHealthChanged(Health, MaxHealth);
	// 		});
	//
	// 	MyCharacter->OnAmmoChanged.AddLambda(
	// 		[this](const int32 Ammo, const int32 MaxAmmo)
	// 		{
	// 			if (HUDWidget.IsValid())
	// 				HUDWidget->OnAmmoChanged(Ammo, MaxAmmo);
	// 		});
	//
	// 	MyCharacter->OnArmorChanged.AddLambda(
	// 		[this](const float Armor, const float MaxArmor)
	// 		{
	// 			if (HUDWidget.IsValid())
	// 				HUDWidget->OnArmorChanged(Armor, MaxArmor);
	// 		});
	//
	// 	FInputModeGameAndUI InputMode;
	// 	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// 	InputMode.SetHideCursorDuringCapture(false);
	// 	SetInputMode(InputMode);
	// 	SetShowMouseCursor(true);
	//
	// 	if (!IsValid(MyRadialMenu) && IsValid(RadialMenuClass))
	// 	{
	// 		MyRadialMenu = CreateWidget<URadialMenuWidget>(this, RadialMenuClass);
	// 		MyRadialMenu->AddToViewport(10);
	// 		MyRadialMenu->GetRadialMenu()->CloseMenu();
	//
	// 		if (!IsValid(DefaultMappingContext) || !IsValid(RadialMenuAction)) return;
	//
	// 		for (const FEnhancedActionKeyMapping& Mapping : DefaultMappingContext->GetMappings())
	// 		{
	// 			if (Mapping.Action == RadialMenuAction)
	// 			{
	// 				MyRadialMenu->GetRadialMenu()->SetSelectionKey(Mapping.Key);
	// 				break;
	// 			}
	// 		}
	//
	// 		if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	// 		{
	// 			EnhancedInput->BindAction(
	// 				RadialMenuAction,
	// 				ETriggerEvent::Started,
	// 				this,
	// 				&AMySlatePlayerController::ShowRadialMenu
	// 			);
	//
	// 			EnhancedInput->BindAction(
	// 				RadialMenuAction,
	// 				ETriggerEvent::Completed,
	// 				this,
	// 				&AMySlatePlayerController::HideAndConfirmRadialMenu
	// 			);
	// 		}
	// 	}
	// }
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

void AMySlatePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AMySlatePlayerController::ShowRadialMenu()
{
	if (!IsValid(MyRadialMenu)) return;
	UE_LOG(LogTemp, Warning, TEXT("SHOW RADIAL MENU"));

	MyRadialMenu->GetRadialMenu()->OpenMenu();
	MyRadialMenu->GetRadialMenu()->SetFocusToMenu();
}

void AMySlatePlayerController::HideAndConfirmRadialMenu()
{
	if (!IsValid(MyRadialMenu)) return;
	UE_LOG(LogTemp, Warning, TEXT("HIDE AND CONFIRM RADIAL MENU"));

	MyRadialMenu->GetRadialMenu()->ConfirmSelection();
	MyRadialMenu->GetRadialMenu()->CloseMenu();
}
