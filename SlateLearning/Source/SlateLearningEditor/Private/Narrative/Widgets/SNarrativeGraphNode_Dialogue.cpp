// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SNarrativeGraphNode_Dialogue.h"

#include "NarrativeGraphNode_Dialogue.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNarrativeGraphNode_Dialogue::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	SNarrativeGraphNode::Construct(SNarrativeGraphNode::FArguments(), InNode);
}

TSharedRef<SWidget> SNarrativeGraphNode_Dialogue::BuildNodeContent()
{
	UNarrativeGraphNode_Dialogue* DialogueNode = Cast<UNarrativeGraphNode_Dialogue>(GraphNode);
	
	if (!DialogueNode) return SNarrativeGraphNode::BuildNodeContent();
	
	return SNew(SBox)
		.WidthOverride(180.f)
		.Padding(FMargin(4.f, 2.f))
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.f, 0.f, 0.f, 2.f)
			[
				SNew(STextBlock)
				.Text_Lambda([DialogueNode]()
				{
					return DialogueNode->SpeakerName.IsEmpty()
					? INVTEXT("No Speaker")
					: DialogueNode->SpeakerName;
				})
				.ColorAndOpacity(FLinearColor(0.7f, 0.9f, 1.0f))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 8))
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text_Lambda([DialogueNode]()
				{
					auto Preview = DialogueNode->DialogueText.ToString();
					if (Preview.Len() > 60)
					{
						Preview = Preview.Left(60) + TEXT("...");
					}
					return Preview.IsEmpty()
					? INVTEXT("(no dialogue text)")
					: FText::FromString(Preview);
				})
				.ColorAndOpacity(FLinearColor(0.8f, 0.8f, 0.8f))
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
				.WrapTextAt(170.f)
			]
		];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
