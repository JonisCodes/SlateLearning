// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DialogueWidget.h"

#include "UI/SDialogueWidget.h"

TSharedRef<SWidget> UDialogueWidget::RebuildWidget()
{
	SlateDialogueWidget = SNew(SDialogueWidget)
		.NarrativeComponent(NarrativeComponent)
		.BackgroundColor(BackgroundColor)
		.SpeakerNameColor(SpeakerNameColor)
		.DialogueTextColor(DialogueTextColor)
		.ChoiceButtonColor(ChoiceButtonColor)
		.ChoiceTextColor(ChoiceTextColor)
		.SpeakerNameFontSize(SpeakerNameFontSize)
		.DialogueTextFontSize(DialogueTextFontSize)
		.ChoiceTextFontSize(ChoiceTextFontSize)
		.TextWrapWidth(TextWrapWidth);
	
	return SlateDialogueWidget.ToSharedRef();
}

void UDialogueWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	SlateDialogueWidget.Reset();
}

void UDialogueWidget::InitializeWithComponent(UNarrativeComponent* InNarrativeComponent)
{
	NarrativeComponent = InNarrativeComponent;
	
	if (SlateDialogueWidget.IsValid())
	{
		SlateDialogueWidget->SetNarrativeComponent(InNarrativeComponent);
	}
}

void UDialogueWidget::ShowDialogue()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UDialogueWidget::HideDialogue()
{
	SetVisibility(ESlateVisibility::Hidden);
}
