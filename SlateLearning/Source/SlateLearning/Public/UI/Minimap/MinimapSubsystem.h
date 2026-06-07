// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MinimapTypes.h"
#include "Engine/SceneCapture2D.h"
#include "Subsystems/Subsystem.h"
#include "MinimapSubsystem.generated.h"

class UMapComponent;
/**
 * 
 */
UCLASS()
class SLATELEARNING_API UMinimapSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return true; }
	virtual TStatId GetStatId() const override;

	void RegisterComponent(UMapComponent* Component);
	void UnregisterComponent(UMapComponent* Component);

	UFUNCTION(BlueprintCallable, Category="Minimap")
	int32 AddPing(const FMinimapPing& Ping);

	UFUNCTION(BlueprintCallable, Category="Minimap")
	void RemovePing(int32 PingId);

	UFUNCTION(BlueprintCallable, Category="Minimap")
	void ClearPings();

	UFUNCTION(BlueprintPure, Category="Minimap")
	USceneCaptureComponent2D* GetCaptureComponent() const
	{
		return IsValid(MapRenderer) ? MapRenderer->GetCaptureComponent2D() : nullptr;
	}

	const TArray<TWeakObjectPtr<UMapComponent>>& GetRegisteredComponents() const { return RegisteredComponents; }
	const TArray<FMinimapPing>& GetPings() const { return Pings; }

	void TickPings(float DeltaTime);

private:
	TArray<TWeakObjectPtr<UMapComponent>> RegisteredComponents;
	TArray<FMinimapPing> Pings;
	int32 NextPingId = 0;
	TObjectPtr<ASceneCapture2D> MapRenderer;

	void RecaptureSceneRenderer() const;
};
