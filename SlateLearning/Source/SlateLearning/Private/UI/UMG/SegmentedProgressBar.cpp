// Fill out your copyright notice in the Description page of Project Settings.


#include "SegmentedProgressBar.h"

#include "SSegmentedProgressBar.h"

void USegmentedProgressBar::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (SegmentedProgressBar.IsValid())
	{
		SegmentedProgressBar->SetPercent(Percent);
		SegmentedProgressBar->SetBackgroundColor(BackgroundColor);
		SegmentedProgressBar->SetNumSegments(NumSegments);
		SegmentedProgressBar->SetFillColor(FillColor);
		SegmentedProgressBar->SetGapSize(GapSize);
		SegmentedProgressBar->SetShearAmount(ShearAmount);
	}
}

TSharedRef<SWidget> USegmentedProgressBar::RebuildWidget()
{
	return SAssignNew(SegmentedProgressBar, SSegmentedProgressBar)
		.Percent(Percent)
		.BackgroundColor(BackgroundColor)
		.NumSegments(NumSegments)
		.FillColor(FillColor)
		.GapSize(GapSize)
		.ShearAmount(ShearAmount);
}

void USegmentedProgressBar::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	SegmentedProgressBar.Reset();
}
