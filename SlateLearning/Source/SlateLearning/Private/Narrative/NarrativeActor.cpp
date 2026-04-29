// Fill out your copyright notice in the Description page of Project Settings.


#include "NarrativeActor.h"

#include "NarrativeComponent.h"
#include "NarrativeNodeData.h"
#include "MySlatePlayerController.h"


// Sets default values
ANarrativeActor::ANarrativeActor()
{
	NarrativeComponent = CreateDefaultSubobject<UNarrativeComponent>(
		TEXT("NarrativeComponent"));
}

// Called when the game starts or when spawned
void ANarrativeActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind events
	NarrativeComponent->OnDialogueNodeReached.AddDynamic(
		this, &ANarrativeActor::OnDialogueReached);

	NarrativeComponent->OnChoiceNodeReached.AddDynamic(
		this, &ANarrativeActor::OnChoiceReached);

	NarrativeComponent->OnNarrativeFinished.AddDynamic(
		this, &ANarrativeActor::OnFinished);
	
	if (const auto PC = GetWorld()->GetFirstPlayerController())
	{
		if (const auto SlatePC = 
			Cast<AMySlatePlayerController>(PC))
		{
			SlatePC->ShowDialogueWidget(NarrativeComponent);
		}
	}
	
	// Start automatically on BeginPlay for testing
	NarrativeComponent->StartNarrative();
}

void ANarrativeActor::OnDialogueReached(
	const FText& SpeakerName, const FText& DialogueText)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: %s"),
		*SpeakerName.ToString(), *DialogueText.ToString());
}

void ANarrativeActor::OnChoiceReached(
	const TArray<FNarrativeNodeData>& Choices)
{
	UE_LOG(LogTemp, Warning, TEXT("Choices available:"));
	for (int32 i = 0; i < Choices.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("  [%d] %s"), i, 
			*Choices[i].ChoiceText.ToString());
	}
}

void ANarrativeActor::OnFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Narrative finished"));
}