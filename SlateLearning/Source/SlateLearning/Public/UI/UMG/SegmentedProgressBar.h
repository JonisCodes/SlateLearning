// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SegmentedProgressBar.generated.h"

class SSegmentedProgressBar;
/**
 * 
 */
UCLASS()
class SLATELEARNING_API USegmentedProgressBar : public UWidget
{
	GENERATED_BODY()

public:
	virtual void SynchronizeProperties() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProgressBar")
	float Percent = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProgressBar")
	FLinearColor BackgroundColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProgressBar")
	FLinearColor FillColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProgressBar")
	int32 NumSegments = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProgressBar")
	float GapSize = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProgressBar")
	float ShearAmount = 0.2f;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	TSharedPtr<SSegmentedProgressBar> SegmentedProgressBar;
};
