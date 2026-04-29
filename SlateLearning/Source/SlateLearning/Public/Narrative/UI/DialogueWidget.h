// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

class UNarrativeComponent;
class SDialogueWidget;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SLATELEARNING_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Dialogue|Style")
	FLinearColor BackgroundColor = FLinearColor(0.f, 0.f, 0.f, 0.7f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Dialogue|Style")
	FLinearColor SpeakerNameColor = FLinearColor(0.4f, 0.8f, 1.f, 1.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Dialogue|Style")
	FLinearColor DialogueTextColor = FLinearColor::White;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Dialogue|Style")
	FLinearColor ChoiceButtonColor = FLinearColor(0.15f, 0.15f, 0.15f, 1.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Dialogue|Style")
	FLinearColor ChoiceTextColor = FLinearColor::White;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Dialogue|Style")
	int32 SpeakerNameFontSize = 14;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Dialogue|Style")
	int32 DialogueTextFontSize = 12;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue|Style")
	int32 ChoiceTextFontSize = 12;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue|Style")
	float TextWrapWidth = 800.f;
	
	// -- Public API --
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void InitializeWithComponent(UNarrativeComponent* InNarrativeComponent);
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ShowDialogue();
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void HideDialogue();
	
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	
private:
	TSharedPtr<SDialogueWidget> SlateDialogueWidget;
	TWeakObjectPtr<UNarrativeComponent> NarrativeComponent;
};
