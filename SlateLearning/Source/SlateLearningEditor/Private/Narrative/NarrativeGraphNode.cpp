// Fill out your copyright notice in the Description page of Project Settings.


#include "NarrativeGraphNode.h"

const FName UNarrativeGraphNode::PinName_Input = FName("Input");
const FName UNarrativeGraphNode::PinName_Output = FName("Output");

UNarrativeGraphNode::UNarrativeGraphNode()
{
}

FText UNarrativeGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return INVTEXT("Narrative Node");
}

FText UNarrativeGraphNode::GetTooltipText() const
{
	return INVTEXT("A narrative graph node");
}

void UNarrativeGraphNode::AllocateDefaultPins()
{
	// Create on input and one output pin by default
	// Subclasses override this to add more pins

	CreatePin(EGPD_Input,
	          FName("exec"),
	          PinName_Input);

	CreatePin(EGPD_Output,
	          FName("exec"),
	          PinName_Output);
}

void UNarrativeGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	if (!FromPin) return;

	for (const auto Pin : Pins)
	{
		if (FromPin->Direction == EGPD_Output && Pin->Direction == EGPD_Input)
		{
			if (const auto Schema = GetGraph()->GetSchema(); Schema->TryCreateConnection(FromPin, Pin))
			{
				break;
			}
		}
		else if (FromPin->Direction == EGPD_Input && Pin->Direction == EGPD_Output)
		{
			if (const auto Schema = GetGraph()->GetSchema(); Schema->TryCreateConnection(FromPin, Pin))
			{
				break;
			}
		}
	}
}

UEdGraphPin* UNarrativeGraphNode::GetOutputPin() const
{
	for (const auto Pin : Pins)
	{
		if (Pin->Direction == EGPD_Output)
			return Pin;
	}
	return nullptr;
}

UEdGraphPin* UNarrativeGraphNode::GetInputPin() const
{
	for (const auto Pin : Pins)
	{
		if (Pin->Direction == EGPD_Input)
			return Pin;
	}
	return nullptr;
}
