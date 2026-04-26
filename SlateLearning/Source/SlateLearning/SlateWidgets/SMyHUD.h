#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class AMySlateCharacter;

class SLATELEARNING_API SMyHUD : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMyHUD) {}
		SLATE_ARGUMENT(TWeakObjectPtr<AMySlateCharacter>, OwningCharacter)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
	
private:
	FCurveSequence PulseSequence;
	FCurveHandle PulseHandle;
	mutable bool bIsLowHealth = false;
	void UpdateHealthState();
	EActiveTimerReturnType TimerCallback(double InCurrentTime, float InDeltaTime);
	
	TOptional<float> GetHealthPercent() const;
	
	float GetArmorPercent() const;
	
	FText GetAmmoText() const;
	
	FSlateColor GetHealthBarColor() const;
	
	TWeakObjectPtr<AMySlateCharacter> OwningCharacter;
	
	FProgressBarStyle BarStyle;
};
