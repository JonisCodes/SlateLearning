// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "StatBar.generated.h"

class SStatBar;
/**
 * 
 */
UCLASS()
class SLATELEARNING_API UStatBar : public UWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetCurrentValue(int32 NewCurrent) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor HighColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor MidColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor LowColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor GhostColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor BackgroundColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowGhostBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DrainSpeed;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	virtual void SynchronizeProperties() override;

private:
	TSharedPtr<SStatBar> MyStatBar;
};
