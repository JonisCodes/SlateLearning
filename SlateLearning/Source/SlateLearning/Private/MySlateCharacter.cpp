// Fill out your copyright notice in the Description page of Project Settings.


#include "MySlateCharacter.h"

#include "SMyHUD.h"
#include "SlateLearning/Structs/FStatRow.h"


// Sets default values
AMySlateCharacter::AMySlateCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

TArray<TSharedPtr<FStatRow>> AMySlateCharacter::GetStatRows() const
{
	return {
	MakeShared<FStatRow>(INVTEXT("Health"),
		FText::AsNumber(Health), Health / MaxHealth),
	MakeShared<FStatRow>(INVTEXT("Armor"),
		FText::AsNumber(Armor), Armor / MaxArmor),
	MakeShared<FStatRow>(INVTEXT("Ammo"),
		FText::AsNumber(Ammo), static_cast<float>(Ammo) / MaxAmmo),
	};
}

void AMySlateCharacter::TakeNewDamage(const float Amount)
{
	Health = FMath::Clamp(Health - Amount, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health, MaxHealth);
}

void AMySlateCharacter::UseAmmo()
{
	Ammo = FMath::Max(0, Ammo - 1);
	OnAmmoChanged.Broadcast(Ammo, MaxAmmo);
}

void AMySlateCharacter::TakeArmorDamage(const float Amount)
{
	Armor = FMath::Clamp(Armor - Amount, 0.f, MaxArmor);
	OnArmorChanged.Broadcast(Armor, MaxArmor);
}

// Called when the game starts or when spawned
void AMySlateCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMySlateCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMySlateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

