// Fill out your copyright notice in the Description page of Project Settings.


#include "StatBar.h"

#include "SStatBar.h"

void UStatBar::SetCurrentValue(const int32 NewCurrent) const
{
	if (MyStatBar.IsValid())
	{
		MyStatBar->SetCurrentValue(NewCurrent);
	}
}

TSharedRef<SWidget> UStatBar::RebuildWidget()
{
	SAssignNew(MyStatBar, SStatBar)
	.HighColor(HighColor)
	.MidColor(MidColor)
	.LowColor(LowColor)
	.GhostColor(GhostColor)
	.BackgroundColor(BackgroundColor)
	.bShowGhostBar(bShowGhostBar)
	.DrainSpeed(DrainSpeed);

	return MyStatBar.ToSharedRef();
}

void UStatBar::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyStatBar.Reset();
}

void UStatBar::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyStatBar.IsValid())
	{
		MyStatBar->SetHighColor(HighColor);
		MyStatBar->SetMidColor(MidColor);
		MyStatBar->SetLowColor(LowColor);
		MyStatBar->SetGhostColor(GhostColor);
		MyStatBar->SetBackgroundColor(BackgroundColor);
		MyStatBar->SetShowGhostBar(bShowGhostBar);
		MyStatBar->SetDrainSpeed(DrainSpeed);
	}
}
