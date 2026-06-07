// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "MinimapWidget.generated.h"

class SMinimapWidget;
/**
 * 
 */
UCLASS()
class SLATELEARNING_API UMinimapWidget : public UWidget
{
	GENERATED_BODY()

public:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void SynchronizeProperties() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minimap")
	float MapRadius = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minimap")
	FVector2D MinimapSize = FVector2D(200.f, 200.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minimap")
	FLinearColor BackgroundColor = FLinearColor(0.f, 0.f, 0.f, 0.5f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minimap")
	TObjectPtr<UTextureRenderTarget2D> MapRenderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minimap")
	FVector2D MapMin = FVector2D(-5000.f, -5000.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minimap")
	FVector2D MapMax = FVector2D(5000.f, 5000.f);

private:
	TSharedPtr<SMinimapWidget> MyMinimapWidget;
};
