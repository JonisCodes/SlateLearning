#pragma once

#include "CoreMinimal.h"
#include "SDamageNumber.h"

class SLATELEARNING_API FDamageNumberPool
	: public TSharedFromThis<FDamageNumberPool>
{
public:
	FDamageNumberPool() = default;
	
	void Initialize(int32 PrewarmCount = 10);
	
	TSharedPtr<SDamageNumber> Acquire(const float DamageAmount, FVector2D ScreenPosition);
	
	void Release(TSharedPtr<SDamageNumber> Widget);
	
	int32 GetActiveCount() const { return ActiveWidgets.Num(); }
	int32 GetPoolSize() const { return InactiveWidgets.Num(); }
	
private:
	TArray<TSharedPtr<SDamageNumber>> ActiveWidgets;
	TArray<TSharedPtr<SDamageNumber>> InactiveWidgets;
	
	int32 MaxPoolSize = 20;
};
