// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SDialogueWidget.h"

#include "SlateOptMacros.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDialogueWidget::Construct(const FArguments& InArgs)
{
	NarrativeComponent = InArgs._NarrativeComponent;
	
	BackgroundColor			= InArgs._BackgroundColor;
	SpeakerNameColor		= InArgs._SpeakerNameColor;
	DialogueTextColor		= InArgs._DialogueTextColor;
	ChoiceButtonColor		= InArgs._ChoiceButtonColor;
	ChoiceTextColor			= InArgs._ChoiceTextColor;
	SpeakerNameFontSize		= InArgs._SpeakerNameFontSize;
	DialogueTextFontSize	= InArgs._DialogueTextFontSize;
	ChoiceTextFontSize		= InArgs._ChoiceTextFontSize;
	TextWrapWidth			= InArgs._TextWrapWidth;
	
	Bridge = NewObject<UDialogueWidgetBridge>();
	Bridge->SlateWidget = SharedThis(this);
	Bridge->AddToRoot();
	
	FadeSequence = FCurveSequence();
	FadeHandle = FadeSequence.AddCurve(0.f, 0.3f, ECurveEaseFunction::CubicOut);
	
	ChildSlot
	[
		SNew(SOverlay)

		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Bottom)
		[
			SNew(SBorder)
			.BorderImage(FCoreStyle::Get().GetBrush("GenericWhiteBox"))
			.BorderBackgroundColor(BackgroundColor)
			.Padding(FMargin(40.f, 20.f))
			.ColorAndOpacity_Lambda([this]()
			{
				return FLinearColor(1.f, 1.f, 1.f,
					FadeSequence.IsPlaying() ? FadeHandle.GetLerp() : 1.f);
			})
			[
				SAssignNew(ContentSwitcher, SWidgetSwitcher)

				+ SWidgetSwitcher::Slot()
				[
					BuildDialoguePanel()
				]

				+ SWidgetSwitcher::Slot()
				[
					BuildChoicePanel()
				]
			]
		]
	];
	
	// Bind to narrative component events
	if (UNarrativeComponent* Comp = NarrativeComponent.Get())
	{
		Comp->OnDialogueNodeReached.AddDynamic(
			Bridge, &UDialogueWidgetBridge::OnDialogueNodeReached);
		Comp->OnChoiceNodeReached.AddDynamic(
			Bridge, &UDialogueWidgetBridge::OnChoiceNodeReached);
		Comp->OnNarrativeFinished.AddDynamic(
			Bridge, &UDialogueWidgetBridge::OnNarrativeFinished);
	}
	
	SetVisibility(EVisibility::Hidden);
}

SDialogueWidget::~SDialogueWidget()
{
	// Unbind delegates
	if (UNarrativeComponent* Comp = NarrativeComponent.Get())
	{
		Comp->OnDialogueNodeReached.RemoveAll(Bridge);
		Comp->OnChoiceNodeReached.RemoveAll(Bridge);
		Comp->OnNarrativeFinished.RemoveAll(Bridge);
	}
	
	if (Bridge)
	{
		Bridge->RemoveFromRoot();
		Bridge = nullptr;
	}
}

void SDialogueWidget::SetNarrativeComponent(UNarrativeComponent* InComponent)
{
	if (UNarrativeComponent* OldComp = NarrativeComponent.Get())
	{
		OldComp->OnDialogueNodeReached.RemoveAll(Bridge);
		OldComp->OnChoiceNodeReached.RemoveAll(Bridge);
		OldComp->OnNarrativeFinished.RemoveAll(Bridge);
	}
	
	NarrativeComponent = InComponent;
	
	if (InComponent && Bridge)
	{
		InComponent->OnDialogueNodeReached.AddDynamic(
			Bridge, &UDialogueWidgetBridge::OnDialogueNodeReached);
		InComponent->OnChoiceNodeReached.AddDynamic(
			Bridge, &UDialogueWidgetBridge::OnChoiceNodeReached);
		InComponent->OnNarrativeFinished.AddDynamic(
			Bridge, &UDialogueWidgetBridge::OnNarrativeFinished);
	}
}

TSharedRef<SWidget> SDialogueWidget::BuildDialoguePanel()
{
	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.OnClicked(this, &SDialogueWidget::OnDialoguePanelClicked)
		[
			SNew(SVerticalBox)

			// Speaker name
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.f, 0.f, 0.f, 8.f)
			[
				SNew(STextBlock)
				.Text_Lambda([this]()
				{
					return CurrentSpeakerName;
				})
				.ColorAndOpacity(SpeakerNameColor)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", SpeakerNameFontSize))
			]

			// dialogue text
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text_Lambda([this]()
				{
					return CurrentDialogueText;
				})
				.ColorAndOpacity(DialogueTextColor)
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", DialogueTextFontSize))
				.WrapTextAt(TextWrapWidth)
			]

			// advance hint
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Right)
			.Padding(0.f, 8.f, 0.f, 0.f)
			[
				SNew(STextBlock)
				.Text(INVTEXT("[ Click to continue ]"))
				.ColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f))
				.Font(FCoreStyle::GetDefaultFontStyle("Italic", 10))
			]
		];
}

TSharedRef<SWidget> SDialogueWidget::BuildChoicePanel()
{
	return SNew(SVerticalBox)
		
		// prompt
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 0.f, 0.f, 12.f)
		[
			SNew(STextBlock)
			.Text(INVTEXT("Choose your response:"))
			.ColorAndOpacity(FLinearColor(0.7f, 0.7f, 0.7f))
			.Font(FCoreStyle::GetDefaultFontStyle("Italic", 11))
		]
		
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(ChoiceBox, SVerticalBox)	
		];
}

void SDialogueWidget::OnDialogueNodeReached(const FText& SpeakerName, const FText& DialogueText)
{
	CurrentSpeakerName = SpeakerName;
	CurrentDialogueText = DialogueText;
	
	ContentSwitcher->SetActiveWidgetIndex(DialoguePanelIndex);
	SetVisibility(EVisibility::Visible);
	
	FadeSequence.Play(this->AsShared(), false, 0.f, true);
}

void SDialogueWidget::OnChoiceNodeReached(const TArray<FNarrativeNodeData>& Choices)
{
	CurrentChoices = Choices;
	
	ChoiceBox->ClearChildren();
	
	for (int32 i = 0; i < Choices.Num(); i++)
	{
		const auto ChoiceIndex = i;
		const auto ChoiceText = Choices[i].ChoiceText;
		
		ChoiceBox->AddSlot()
		.AutoHeight()
		.Padding(0.f, 4.f)
		[
			SNew(SButton)
			.OnClicked_Lambda([this, ChoiceIndex]()
			{
				return OnChoiceSelected(ChoiceIndex);
			})
			.ButtonColorAndOpacity(ChoiceButtonColor)
			[
				SNew(STextBlock)
				.Text(ChoiceText)
				.ColorAndOpacity(ChoiceTextColor)
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", ChoiceTextFontSize))
				.Margin(FMargin(8.f, 4.f))
			]
		];
	}
	
	ContentSwitcher->SetActiveWidgetIndex(ChoicePanelIndex);
	SetVisibility(EVisibility::Visible);
}

void SDialogueWidget::OnNarrativeFinished()
{
	SetVisibility(EVisibility::Hidden);
}

FReply SDialogueWidget::OnDialoguePanelClicked()
{
	if (const auto Comp = NarrativeComponent.Get())
	{
		Comp->AdvanceDialogue();
	}
	return FReply::Handled();
}

FReply SDialogueWidget::OnChoiceSelected(int32 ChoiceIndex)
{
	if (const auto Comp = NarrativeComponent.Get())
	{
		Comp->SelectChoice(ChoiceIndex);
	}
	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
