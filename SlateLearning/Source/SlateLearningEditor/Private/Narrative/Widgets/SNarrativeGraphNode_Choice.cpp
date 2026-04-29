// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SNarrativeGraphNode_Choice.h"

#include "NarrativeGraphNode_Choice.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNarrativeGraphNode_Choice::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	SNarrativeGraphNode::Construct(SNarrativeGraphNode::FArguments(), InNode);
}

TSharedRef<SWidget> SNarrativeGraphNode_Choice::BuildNodeContent()
{
	UNarrativeGraphNode_Choice* ChoiceNode = Cast<UNarrativeGraphNode_Choice>(GraphNode);

	if (!ChoiceNode) return SNarrativeGraphNode::BuildNodeContent();

	return SNew(SBox)
		.WidthOverride(180.f)
		.Padding(FMargin(4.f, 2.f))
		[
			SNew(SVerticalBox)

			// Choice text
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.f, 0.f, 0.f, 2.f)
			[
				SNew(STextBlock)
				.Text_Lambda([ChoiceNode]()
				{
					return ChoiceNode->ChoiceText.IsEmpty()
						       ? INVTEXT("(no choice text)")
						       : ChoiceNode->ChoiceText;
				})
				.ColorAndOpacity(FLinearColor(0.9f, 0.85f, 1.0f))
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
				.WrapTextAt(170.f)
			]
    
			// Condition indicator - only shown when condition is set
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				.Visibility_Lambda([ChoiceNode]()
				{
					return ChoiceNode->Condition.IsEmpty()
						       ? EVisibility::Collapsed
						       : EVisibility::Visible;
				})

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(0.f, 0.f, 4.f, 0.f)
				[
					SNew(STextBlock)
					.Text(INVTEXT("IF:"))
					.ColorAndOpacity(FLinearColor(1.0f, 0.8f, 0.2f))
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 7))
				]

				+ SHorizontalBox::Slot()
				.FillWidth(1.f)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text_Lambda([ChoiceNode]()
					{
						return ChoiceNode->Condition;
					})
					.ColorAndOpacity(FLinearColor(1.0f, 0.8f, 0.2f))
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 7))
					.WrapTextAt(130.f)
				]
			]
		];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
