#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SArmorRing;
struct FStatRow;
class AMySlateCharacter;

class SLATELEARNING_API SMyHUD : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMyHUD) {}
		SLATE_ARGUMENT(TWeakObjectPtr<AMySlateCharacter>, OwningCharacter)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
	
	void OnHealthChanged(const float NewHealth, const float MaxHealth);
	void OnAmmoChanged(int32 NewAmmo, int32 MaxAmmo);
	void OnArmorChanged(float NewArmor, float MaxArmor);
	
private:
	
	// Cached values, only invalidate when these change
	float CachedHealthPercent = 1.f;
	float CachedArmorPercent = 1.f; 
	FText CachedAmmoText;
	
	FCurveSequence PulseSequence;
	FCurveHandle PulseHandle;
	mutable bool bIsLowHealth = false;
	void UpdateHealthState();
	EActiveTimerReturnType TimerCallback(double InCurrentTime, float InDeltaTime);
	
	TOptional<float> GetHealthPercent() const;
	
	TArray<TSharedPtr<FStatRow>> GetStatRows() const;
	
	float GetArmorPercent() const;
	
	FText GetAmmoText() const;
	
	FSlateColor GetHealthBarColor() const;
	
	TWeakObjectPtr<AMySlateCharacter> OwningCharacter;
	
	TSharedPtr<SArmorRing> ArmorRingWidget;
	
};
