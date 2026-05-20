// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimatedButton.h"

#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"

void UAnimatedButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(MyButton))
	{
		MyButton->OnPressed.AddDynamic(this, &UAnimatedButton::OnMyButtonPressed);
	}
}


void UAnimatedButton::OnMyButtonPressed()
{
	if (IsValid(PressAnimation))
	{
		PlayAnimationForward(PressAnimation, 1 / PressDuration);
	}
}
