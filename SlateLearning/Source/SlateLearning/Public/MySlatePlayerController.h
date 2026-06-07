// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NarrativeComponent.h"
#include "UI/DialogueWidget.h"
#include "EnhancedInputSubSystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "MySlatePlayerController.generated.h"

class URadialMenuWidget;
class URadialMenu;
class SMyHUD;
/**
 * 
 */
UCLASS()
class SLATELEARNING_API AMySlatePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void ShowDialogueWidget(UNarrativeComponent* NarrativeComp);

	UFUNCTION()
	void OnNarrativeStarted();

	UFUNCTION()
	void OnNarrativeFinished();

	UPROPERTY()
	UDialogueWidget* DialogueWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<URadialMenuWidget> RadialMenuClass;

	UPROPERTY()
	TObjectPtr<URadialMenuWidget> MyRadialMenu;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input|RadialMenu")
	TObjectPtr<UInputAction> RadialMenuAction;

protected:
	virtual void SetupInputComponent() override;

	void ShowRadialMenu();
	void HideAndConfirmRadialMenu();

private:
	// TSharedPtr<SDialogueWidget> DialogueWidget;
	TSharedPtr<SMyHUD> HUDWidget;
};
