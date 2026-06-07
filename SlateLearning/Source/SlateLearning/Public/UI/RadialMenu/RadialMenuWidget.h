// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RadialMenuWidget.generated.h"

struct FRadialMenuItem;
class URadialMenu;
/**
 * 
 */
UCLASS()
class SLATELEARNING_API URadialMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<URadialMenu> MyRadialMenu;

	TObjectPtr<URadialMenu> GetRadialMenu() { return MyRadialMenu; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRadialMenuItem> Segments;
};
