// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */

DECLARE_DELEGATE(FOnDamageNumberFinished);

class SLATELEARNING_API SDamageNumber : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDamageNumber){}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
	void Activate(const float DamageAmount, FVector2D ScreenPosition);
	
	FOnDamageNumberFinished OnFinished;
	
private:
	EActiveTimerReturnType AnimationTick(
		double CurrentTime, float DeltaTime);
	
	float CurrentDamage = 0.f;
	float Lifetime = 0.f;
	float MaxLifetime = 1.5f;
	
	FVector2D StartPosition;
	FVector2D CurrentPosition;
	
	FCurveSequence FadeSequence;
	FCurveHandle FadeHandle;
	
	TSharedPtr<STextBlock> DamageText;
	TSharedPtr<SBox> PositionBox;
};
