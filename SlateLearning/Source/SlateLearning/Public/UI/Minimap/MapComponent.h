// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MinimapTypes.h"
#include "Components/ActorComponent.h"
#include "MapComponent.generated.h"


enum class EMapActorType : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SLATELEARNING_API UMapComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMapComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minimap")
	EMapActorType ActorType = EMapActorType::Enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minimap")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minimap")
	FLinearColor IconColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minimap")
	float IconSize = 16.f;

	/* Hides the actor itself from the map renderer  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minimap")
	bool bHideFromSceneCapture = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
