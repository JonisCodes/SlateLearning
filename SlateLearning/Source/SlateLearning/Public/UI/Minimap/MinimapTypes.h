// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MinimapTypes.generated.h"

UENUM(BlueprintType)
enum class EMapActorType : uint8
{
	Player,
	Enemy,
	NPC,
	Objective,
	Ping
};

USTRUCT(BlueprintType)
struct FMinimapPing
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector WorldLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Size = 12.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 3.f;

	float CreatedTime = 0.f;
	int32 PingId = INDEX_NONE;
};
