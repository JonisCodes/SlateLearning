// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "RadialMenu.generated.h"

struct FRadialMenuItem;
class SRadialMenu;
/**
 * 
 */
UCLASS()
class SLATELEARNING_API URadialMenu : public UWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHandleSegmentSelected, const FRadialMenuItem&, SegmentIndex);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHandleSegmentHovered, const FRadialMenuItem&, SegmentItem);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHandleSegmentUnhovered, const FRadialMenuItem&, SegmentItem);


	virtual void SynchronizeProperties() override;

	UPROPERTY(BlueprintAssignable, Category="RadialMenu|Event")
	FOnHandleSegmentSelected OnSelect;

	UPROPERTY(BlueprintAssignable, Category="RadialMenu|Event")
	FOnHandleSegmentHovered OnHovered;

	UPROPERTY(BlueprintAssignable, Category="RadialMenu|Event")
	FOnHandleSegmentUnhovered OnUnhovered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialMenu")
	int32 NumSegments = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialMenu")
	float MenuOuterDiameter = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialMenu")
	float MenuInnerDiameter = 120.f;

	UFUNCTION(BlueprintCallable, Category = "RadialMenu")
	void AddItem(const FLinearColor Color, const FLinearColor HoveredColor, const FText Label, const bool bIsValid,
	             const FSlateBrush Brush) const;

	UFUNCTION(BlueprintCallable, Category = "RadialMenu")
	void OpenMenu() const;

	UFUNCTION(BlueprintCallable, Category = "RadialMenu")
	void CloseMenu() const;

	UFUNCTION(BlueprintCallable)
	void SetFocusToMenu() const;

	UFUNCTION()
	void SetSelectionKey(const FKey& NewSelectionKey);

	void ConfirmSelection() const;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UFUNCTION()
	void HandleSegmentSelected(const FRadialMenuItem& SelectedItem) const;

	UFUNCTION()
	void HandleSegmentHovered(const FRadialMenuItem& HoveredItem) const;

	UFUNCTION()
	void HandleSegmentUnhovered(const FRadialMenuItem& UnhoveredItem) const;

private:
	TSharedPtr<SRadialMenu> MyRadialMenu;

	FKey SelectionKey = EKeys::E;
};
