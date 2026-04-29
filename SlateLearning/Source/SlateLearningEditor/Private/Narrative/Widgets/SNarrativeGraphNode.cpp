// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SNarrativeGraphNode.h"
#include "NarrativeGraphNode.h"
#include "NodeFactory.h"
#include "SGraphPin.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNarrativeGraphNode::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	GraphNode = InNode;
	SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
}

void SNarrativeGraphNode::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	LeftNodeBox.Reset();
	RightNodeBox.Reset();


	GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("Graph.Node.Body"))
			.BorderBackgroundColor(FLinearColor(0.1f, 0.1f, 0.1f))
			.Padding(0)
			[
				SNew(SVerticalBox)

				// Title bar
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SBorder)
					.BorderImage(FAppStyle::GetBrush("Graph.Node.TitleBackground"))
					.BorderBackgroundColor(GraphNode->GetNodeTitleColor())
					.Padding(FMargin(8.f, 4.f))
					[
						SNew(STextBlock)
						.Text(GraphNode->GetNodeTitle(ENodeTitleType::FullTitle))
						.ColorAndOpacity(FLinearColor::White)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
					]
				]

				// Content area
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(4.f)
				[
					BuildNodeContent()
				]

				// Pin area
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SAssignNew(LeftNodeBox, SVerticalBox)
					]

					+ SHorizontalBox::Slot()
					.FillWidth(1.f)

					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SAssignNew(RightNodeBox, SVerticalBox)
					]
				]
			]
		];

	CreatePinWidgets();
}

void SNarrativeGraphNode::CreatePinWidgets()
{
	for (const auto Pin : GraphNode->Pins)
	{
		if (const auto NewPin = FNodeFactory::CreatePinWidget(Pin); NewPin.IsValid())
		{
			AddPin(NewPin.ToSharedRef());
		}
	}
}

void SNarrativeGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	if (PinToAdd->GetDirection() == EGPD_Input)
	{
		LeftNodeBox->AddSlot()
		           .AutoHeight()
		           .HAlign(HAlign_Left)
		[
			PinToAdd
		];
		InputPins.Add(PinToAdd);
	}
	else
	{
		RightNodeBox->AddSlot()
		            .AutoHeight()
		            .HAlign(HAlign_Right)
		[
			PinToAdd
		];
		OutputPins.Add(PinToAdd);
	}
}

TSharedRef<SWidget> SNarrativeGraphNode::BuildNodeContent()
{
	return SNew(SBox).HeightOverride(4.f);
}

UNarrativeGraphNode* SNarrativeGraphNode::GetNarrativeNode() const
{
	return Cast<UNarrativeGraphNode>(GraphNode);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
