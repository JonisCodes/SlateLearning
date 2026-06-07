#include "Minimap/MapComponent.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Minimap/MinimapSubsystem.h"

UMapComponent::UMapComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UMapComponent::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	UMinimapSubsystem* MinimapSubsystem = World->GetSubsystem<UMinimapSubsystem>();
	if (!IsValid(MinimapSubsystem)) return;

	MinimapSubsystem->RegisterComponent(this);

	if (USceneCaptureComponent2D* Capture = MinimapSubsystem->GetCaptureComponent())
	{
		Capture->ShowOnlyActors.Add(GetOwner());
	}
}

void UMapComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	const TObjectPtr<UWorld> World = GetWorld();
	if (!IsValid(World)) return;

	const TObjectPtr<UMinimapSubsystem> MinimapSubsystem = World->GetSubsystem<UMinimapSubsystem>();
	if (!IsValid(MinimapSubsystem)) return;

	MinimapSubsystem->UnregisterComponent(this);
}


void UMapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
