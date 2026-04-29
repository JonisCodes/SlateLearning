// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MySlateCharacter.generated.h"

class SMyHUD;
struct FStatRow;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnArmorChanged, float, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, int32, int32);

UCLASS()
class SLATELEARNING_API AMySlateCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMySlateCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Ammo = 15;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxAmmo = 15;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Armor = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxArmor = 100.f;
	
	TArray<TSharedPtr<FStatRow>> GetStatRows() const;
	
	UFUNCTION(BlueprintCallable)
	void TakeNewDamage(const float Amount);
	
	UFUNCTION(BlueprintCallable)
	void UseAmmo();
	
	UFUNCTION(BlueprintCallable)
	void TakeArmorDamage(const float Amount);
	
	FOnHealthChanged OnHealthChanged;
	FOnAmmoChanged OnAmmoChanged;
	FOnArmorChanged OnArmorChanged;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
