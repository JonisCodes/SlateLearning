// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NarrativeComponent.h"
#include "UI/DialogueWidget.h"
#include "MySlatePlayerController.generated.h"

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
	
	
private:
	// TSharedPtr<SDialogueWidget> DialogueWidget;
	TSharedPtr<SMyHUD> HUDWidget;
	
	
};
