// Fill out your copyright notice in the Description page of Project Settings.

#include "MySlatePlayerController.h"

#include "MySlateCharacter.h"
#include "../SlateWidgets/SMyHUD.h"
#include "Engine/GameViewportClient.h"

void AMySlatePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (GEngine && GEngine->GameViewport)
	{
		const auto MySlateHUD = SNew(SMyHUD).OwningCharacter(Cast<AMySlateCharacter>(GetPawn()));
		GEngine->GameViewport->AddViewportWidgetContent(MySlateHUD, 10);
	}
}
