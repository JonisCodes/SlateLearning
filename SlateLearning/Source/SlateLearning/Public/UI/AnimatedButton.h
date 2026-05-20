// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AnimatedButton.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class SLATELEARNING_API UAnimatedButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	float PressDuration = 0.5f;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> MyButton;

	UPROPERTY(Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PressAnimation;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnMyButtonPressed();
};
