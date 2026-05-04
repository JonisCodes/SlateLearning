#include "FDamageNumberPool.h"

#include "PrintObjectUtils.h"

void FDamageNumberPool::Initialize(int32 PrewarmCount)
{
	for (int32 i = 0; i < PrewarmCount; i++)
	{
		TSharedPtr<SDamageNumber> NewWidget = SNew(SDamageNumber);
		InactiveWidgets.Add(NewWidget);
	}
	
	UE_LOG(LogTemp, Log, TEXT("DamageNumberPool: Prewarmed with %d widgets"),
		PrewarmCount);
}

TSharedPtr<SDamageNumber> FDamageNumberPool::Acquire(const float DamageAmount, FVector2D ScreenPosition)
{
	TSharedPtr<SDamageNumber> Widget;
	
	if (InactiveWidgets.Num() > 0)
	{
		//  reuse from pool
		Widget = InactiveWidgets.Last();
		InactiveWidgets.RemoveAt(InactiveWidgets.Num() - 1);
		
		UE_LOG(LogTemp, Verbose,
			TEXT("DamageNumberPool: Reusing widget. Pool size: %d"),
			InactiveWidgets.Num());
	}
	else if (ActiveWidgets.Num() < MaxPoolSize)
	{
		// pool exhausted but under max, create new
		Widget = SNew(SDamageNumber);
		UE_LOG(LogTemp, Warning,
			TEXT("DamageNumberPool: Pool exhausted, creating new widget"));
	}
	else
	{
		// At max capacity, steal the oldest active widget
		Widget = ActiveWidgets[0];
		ActiveWidgets.RemoveAt(0);
		
		UE_LOG(LogTemp, Warning,
			TEXT("DamageNumberPool: At max capacity, stealing oldest widget"));
	}
	
	Widget->OnFinished.BindSP(
		this, &FDamageNumberPool::Release, Widget);
	
	Widget->Activate(DamageAmount, ScreenPosition);
	ActiveWidgets.Add(Widget);
	
	return Widget;
}

void FDamageNumberPool::Release(TSharedPtr<SDamageNumber> Widget)
{
	ActiveWidgets.Remove(Widget);
	
	if (InactiveWidgets.Num() < MaxPoolSize)
	{
		InactiveWidgets.Add(Widget);
	}
	
	// if over max just let it go TSharedPtr handles cleanup
	UE_LOG(LogTemp, Verbose,
		TEXT("DamageNumberPool: Released widget. Pool size: %d"),
		InactiveWidgets.Num());
}


