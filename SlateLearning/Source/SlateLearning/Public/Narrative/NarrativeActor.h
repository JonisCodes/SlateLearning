#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NarrativeActor.generated.h"

class UNarrativeComponent;

UCLASS()
class SLATELEARNING_API ANarrativeActor : public AActor
{
	GENERATED_BODY()

public:
	ANarrativeActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNarrativeComponent* NarrativeComponent;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnDialogueReached(const FText& SpeakerName, const FText& DialogueText);

	UFUNCTION()
	void OnChoiceReached(const TArray<FNarrativeNodeData>& Choices);

	UFUNCTION()
	void OnFinished();
};