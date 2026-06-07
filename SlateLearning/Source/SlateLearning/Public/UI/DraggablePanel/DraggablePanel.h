// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SDraggablePanel.h"
#include "Components/ContentWidget.h"
#include "DraggablePanel.generated.h"

/**
 * 
 */

UCLASS()
class SLATELEARNING_API UDraggablePanel : public UContentWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWidgetDragged, const FVector2D, Position);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClosePanel);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPanelBroughtToFront);

	UPROPERTY(BlueprintAssignable, Category="DraggablePanel|Events")
	FOnWidgetDragged OnWidgetDragged;

	UPROPERTY(BlueprintAssignable, Category="DraggablePanel|Events")
	FOnClosePanel OnClosePanel;

	UPROPERTY(BlueprintAssignable, Category="DraggablePanel|Events")
	FOnPanelBroughtToFront OnPanelBroughtToFront;

	UPROPERTY(EditAnywhere, Category="DraggablePanel|TitleBar")
	FLinearColor BackgroundColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, Category="DraggablePanel|TitleBar")
	FSlateBrush BackgroundBrush;

	UPROPERTY(EditAnywhere, Category="DraggablePanel|TitleBar|Text")
	FMargin TextPadding;

	UPROPERTY(EditAnywhere, Category="DraggablePanel|TitleBar|Text")
	FText TitleText;

	UPROPERTY(EditAnywhere, Category="DraggablePanel|TitleBar|Text")
	FLinearColor TextColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, Category="DraggablePanel|TitleBar|Text")
	FSlateFontInfo TextFont;

	UPROPERTY(EditAnywhere, Category="DraggablePanel|TitleBar|CloseButton")
	FVector2D CloseButtonSize = FVector2D(32.f, 32.f);

	UPROPERTY(EditAnywhere, Category="DraggablePanel|TitleBar|CloseButton")
	FMargin CloseButtonContentPadding = FMargin(0.f, 7.f);

	UPROPERTY(EditAnywhere, Category="DraggablePanel|TitleBar|CloseButton")
	FLinearColor CloseButtonColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, Category="DraggablePanel|TitleBar|CloseButton")
	FLinearColor CloseButtonTextColor = FLinearColor::Black;

	UPROPERTY(EditAnywhere, Category="DraggablePanel|TitleBar|CloseButton")
	FButtonStyle CloseButtonStyle;

	void HandleOnDragged(FVector2D Position) const;
	void HandleOnClosePanel();

	virtual void SynchronizeProperties() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void OnSlotAdded(UPanelSlot* InSlot) override;
	virtual void OnSlotRemoved(UPanelSlot* InSlot) override;

	int32 GetCurrentZOrder() const;
	void OnPanelFocused() const;
	void OnPanelUnfocused() const;

private:
	TSharedPtr<SDraggablePanel> MyDraggablePanel;

	int32 FocusZOrder = 100;
	int32 BaseZOrder = 0;
};
