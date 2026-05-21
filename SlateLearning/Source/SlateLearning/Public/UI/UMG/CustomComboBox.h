// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FComboBoxItem.h"
#include "Components/Widget.h"
#include "CustomComboBox.generated.h"

class SCustomComboBox;
/**
 * 
 */

UCLASS()
class SLATELEARNING_API UCustomComboBox : public UWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboBoxSelectionChanged, const FComboBoxItem&, Item);

	UPROPERTY(BlueprintAssignable, Category="ComboBox|Event")
	FOnComboBoxSelectionChanged OnSelectionChanged;

	virtual void SynchronizeProperties() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ComboBox")
	float DesiredWidth = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ComboBox|Header")
	float HeaderHeight = 24.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ComboBox|Panel")
	float PanelHeight = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ComboBox|Panel")
	float SearchBoxHeight = 24.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ComboBox|Panel")
	FLinearColor PanelColor = FLinearColor(1.f, 1.f, 1.f, 0.5f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ComboBox|Panel")
	FSlateBrush PanelBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ComboBox|Panel")
	bool bOpenComboBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ComboBox|Panel|Row")
	FSlateFontInfo RowFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ComboBox|Header")
	FLinearColor HeaderColor = FLinearColor(1.f, 1.f, 1.f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ComboBox|Header")
	FLinearColor HeaderFontColor = FLinearColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ComboBox|Header")
	FLinearColor HeaderArrowColor = FLinearColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ComboBox|Header")
	FSlateBrush HeaderBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ComboBox|Header")
	FSlateFontInfo HeaderFont;

	UFUNCTION(BlueprintCallable)
	bool AddItem(const FLinearColor BackgroundColor, const FText DisplayName, FLinearColor TextColor);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UFUNCTION()
	void HandleSelectionChanged(const FComboBoxItem& SelectedItem);

private:
	TSharedPtr<SCustomComboBox> CustomComboBox;
};
