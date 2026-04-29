// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DialogueWidgetBridge.h"

#include "UI/SDialogueWidget.h"

void UDialogueWidgetBridge::OnDialogueNodeReached(const FText& SpeakerName, const FText& DialogueText)
{
	if (const TSharedPtr<SDialogueWidget> Widget = SlateWidget.Pin())
		Widget->OnDialogueNodeReached(SpeakerName, DialogueText);
}

void UDialogueWidgetBridge::OnChoiceNodeReached(const TArray<FNarrativeNodeData>& Choices)
{
	if (const TSharedPtr<SDialogueWidget> Widget = SlateWidget.Pin())
		Widget->OnChoiceNodeReached(Choices);
}

void UDialogueWidgetBridge::OnNarrativeFinished()
{
	if (const TSharedPtr<SDialogueWidget> Widget = SlateWidget.Pin())
		Widget->OnNarrativeFinished();
}
