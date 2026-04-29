// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NarrativeComponent.h"
#include "DialogueWidgetBridge.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SLATELEARNING_API SDialogueWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialogueWidget)
		: _BackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.7f))
		, _SpeakerNameColor(FLinearColor(0.4f, 0.8f, 1.f, 1.f))
		, _DialogueTextColor(FLinearColor::White)
		, _ChoiceButtonColor(FLinearColor(0.15f, 0.15f, 0.15f, 1.f))
		, _ChoiceTextColor(FLinearColor::White)
		, _SpeakerNameFontSize(14)
		, _DialogueTextFontSize(12)
		, _ChoiceTextFontSize(12)
		, _TextWrapWidth(800.f)
		{}

		SLATE_ARGUMENT(TWeakObjectPtr<UNarrativeComponent>, NarrativeComponent)
		SLATE_ARGUMENT(FLinearColor, BackgroundColor)
		SLATE_ARGUMENT(FLinearColor, SpeakerNameColor)
		SLATE_ARGUMENT(FLinearColor, DialogueTextColor)
		SLATE_ARGUMENT(FLinearColor, ChoiceButtonColor)
		SLATE_ARGUMENT(FLinearColor, ChoiceTextColor)
		SLATE_ARGUMENT(int32, SpeakerNameFontSize)
		SLATE_ARGUMENT(int32, DialogueTextFontSize)
		SLATE_ARGUMENT(int32, ChoiceTextFontSize)
		SLATE_ARGUMENT(float, TextWrapWidth)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual ~SDialogueWidget() override;
	
	// delegate callbacks
	UFUNCTION()
	void OnDialogueNodeReached(const FText& SpeakerName, const FText& DialogueText);
	
	UFUNCTION()
	void OnChoiceNodeReached(const TArray<FNarrativeNodeData>& Choices);
	
	UFUNCTION()
	void OnNarrativeFinished();
	
	UPROPERTY()
	UDialogueWidgetBridge* Bridge = nullptr;
	
	void SetNarrativeComponent(UNarrativeComponent* InComponent);
	
private:
	// Build layout section
	TSharedRef<SWidget> BuildDialoguePanel();
	TSharedRef<SWidget> BuildChoicePanel();
	
	FReply OnChoiceSelected(int32 ChoiceIndex);
	
	FReply OnDialoguePanelClicked();
	
	FText CurrentSpeakerName;
	FText CurrentDialogueText;
	TArray<FNarrativeNodeData> CurrentChoices;
	
	TSharedPtr<SWidgetSwitcher> ContentSwitcher;
	TSharedPtr<SVerticalBox> ChoiceBox;
	
	FCurveSequence FadeSequence;
	FCurveHandle FadeHandle;
	
	TWeakObjectPtr<UNarrativeComponent> NarrativeComponent;
	
	static const int32 DialoguePanelIndex = 0;
	static const int32 ChoicePanelIndex = 1;
	
	FLinearColor BackgroundColor;
	FLinearColor SpeakerNameColor;
	FLinearColor DialogueTextColor;
	FLinearColor ChoiceButtonColor;
	FLinearColor ChoiceTextColor;
	int32 SpeakerNameFontSize;
	int32 DialogueTextFontSize;
	int32 ChoiceTextFontSize;
	float TextWrapWidth;
};
