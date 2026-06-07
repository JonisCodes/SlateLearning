#include "Minimap/MinimapSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/SceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"

#include "Minimap/MapComponent.h"

void UMinimapSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	ASceneCapture2D* CaptureActor = Cast<ASceneCapture2D>(
		UGameplayStatics::GetActorOfClass(&InWorld, ASceneCapture2D::StaticClass()));

	if (IsValid(CaptureActor))
	{
		MapRenderer = CaptureActor;
	}
}

bool UMinimapSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::PIE || WorldType == EWorldType::Game;
}

void UMinimapSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickPings(DeltaTime);
}

TStatId UMinimapSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMinimapSubsystem, STATGROUP_Tickables);
}

void UMinimapSubsystem::RegisterComponent(UMapComponent* Component)
{
	if (!IsValid(Component)) return;

	RegisteredComponents.Add(Component);
	RecaptureSceneRenderer();
}

void UMinimapSubsystem::UnregisterComponent(UMapComponent* Component)
{
	if (!IsValid(Component)) return;

	RegisteredComponents.Remove(Component);
	RecaptureSceneRenderer();
}

int32 UMinimapSubsystem::AddPing(const FMinimapPing& Ping)
{
	const UWorld* World = GetWorld();

	if (!IsValid(World)) return -1;

	FMinimapPing NewPing = Ping;
	NewPing.CreatedTime = World->GetTimeSeconds();
	NewPing.PingId = NextPingId++;
	Pings.Add(NewPing);

	RecaptureSceneRenderer();

	return NewPing.PingId;
}

void UMinimapSubsystem::RemovePing(const int32 PingId)
{
	Pings.RemoveAll([PingId](const FMinimapPing& Ping)
	{
		return Ping.PingId == PingId;
	});

	RecaptureSceneRenderer();
}

void UMinimapSubsystem::ClearPings()
{
	Pings.Empty();
}

void UMinimapSubsystem::TickPings(float DeltaTime)
{
	const UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	Pings.RemoveAll([World](const FMinimapPing& Ping)
	{
		if (Ping.Duration == 0.f) return false;
		return World->GetTimeSeconds() - Ping.CreatedTime >= Ping.Duration;
	});
}

void UMinimapSubsystem::RecaptureSceneRenderer() const
{
	if (!IsValid(MapRenderer)) return;

	USceneCaptureComponent2D* CaptureComponent = MapRenderer->GetCaptureComponent2D();
	if (!IsValid(CaptureComponent)) return;

	CaptureComponent->CaptureScene();
	UE_LOG(LogTemp, Warning, TEXT("Recapture scene renderer"));
}
