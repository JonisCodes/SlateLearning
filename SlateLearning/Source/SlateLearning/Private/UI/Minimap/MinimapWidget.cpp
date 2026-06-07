// Fill out your copyright notice in the Description page of Project Settings.


#include "Minimap/MinimapWidget.h"
#include "Minimap/MinimapSubsystem.h"
#include "Minimap/SMinimapWidget.h"

TSharedRef<SWidget> UMinimapWidget::RebuildWidget()
{
	UMinimapSubsystem* MinimapSubsystem = nullptr;

	if (const UWorld* World = GetWorld())
	{
		MinimapSubsystem = World->GetSubsystem<UMinimapSubsystem>();
	}

	SAssignNew(MyMinimapWidget, SMinimapWidget)
	.MinimapSubsystem(MinimapSubsystem)
	.MinimapSize(MinimapSize)
	.MapRadius(MapRadius)
	.BackgroundColor(BackgroundColor)
	.MapRenderTarget(MapRenderTarget)
	.MapMin(MapMin)
	.MapMax(MapMax);

	MyMinimapWidget->SetRenderTarget(MapRenderTarget.Get());

	return MyMinimapWidget.ToSharedRef();
}

void UMinimapWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyMinimapWidget.Reset();
}

void UMinimapWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (MyMinimapWidget.IsValid())
	{
		MyMinimapWidget->SetMinimapSize(MinimapSize);
		MyMinimapWidget->SetMapRadius(MapRadius);
		MyMinimapWidget->SetBackgroundColor(BackgroundColor);
		MyMinimapWidget->SetRenderTarget(MapRenderTarget);
		MyMinimapWidget->SetMapMin(MapMin);
		MyMinimapWidget->SetMapMax(MapMax);
	}
}
