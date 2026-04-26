// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MySlateCharacter.generated.h"

UCLASS()
class SLATELEARNING_API AMySlateCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMySlateCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 100.f;
	
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Ammo = 15;
	
	UPROPERTY(EditAnywhere)
	int MaxAmmo = 15;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Armor = 100.f;
	
	UPROPERTY(EditAnywhere)
	float MaxArmor = 100.f;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
