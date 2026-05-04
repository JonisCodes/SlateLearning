// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SDamageNumber.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDamageNumber::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SAssignNew(PositionBox, SBox)
		[
			SAssignNew(DamageText, STextBlock)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18))
			.ColorAndOpacity_Lambda([this]()
			{
				const auto Alpha = 1.f - FadeHandle.GetLerp();
				return FSlateColor(FLinearColor(1.f, 0.2f, 0.2f, Alpha));
			})
		]
	];
	
	SetVisibility(EVisibility::Collapsed);
}

void SDamageNumber::Activate(const float DamageAmount, FVector2D ScreenPosition)
{
	CurrentDamage = DamageAmount;
	StartPosition = ScreenPosition;
	CurrentPosition = ScreenPosition;
	Lifetime = 0.f;
	
	DamageText->SetText(FText::AsNumber(FMath::RoundToInt(DamageAmount)));
	
	SetVisibility(EVisibility::HitTestInvisible);
	
	FadeSequence.Play(this->AsShared(), false, 0.f, true);
	
	RegisterActiveTimer(0.f,
		FWidgetActiveTimerDelegate::CreateSP(
			this, &SDamageNumber::AnimationTick));
}

EActiveTimerReturnType SDamageNumber::AnimationTick(double CurrentTime, float DeltaTime)
{
	Lifetime += DeltaTime;
	
	CurrentPosition.Y = StartPosition.Y - (Lifetime * 60.f);
	
	Invalidate(EInvalidateWidgetReason::Paint);
	
	if (Lifetime >= MaxLifetime)
	{
		SetVisibility(EVisibility::Collapsed);
		FadeSequence.Pause();
		OnFinished.ExecuteIfBound();
		return EActiveTimerReturnType::Stop;
	}
	
	return EActiveTimerReturnType::Continue;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
