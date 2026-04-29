#include "FNarrativeGraphNodeFactory.h"

#include "NarrativeGraphNode_Choice.h"
#include "NarrativeGraphNode_Dialogue.h"
#include "Widgets/SNarrativeGraphNode_Choice.h"
#include "Widgets/SNarrativeGraphNode_Dialogue.h"

TSharedPtr<SGraphNode> FNarrativeGraphNodeFactory::CreateNode(UEdGraphNode* InNode) const
{
	if (const auto DialogueNode = Cast<UNarrativeGraphNode_Dialogue>(InNode))
	{
		return SNew(SNarrativeGraphNode_Dialogue, DialogueNode);
	}
	
	if (const auto ChoiceNode = Cast<UNarrativeGraphNode_Choice>(InNode))
	{
		return SNew(SNarrativeGraphNode_Choice, ChoiceNode);
	}
	
	if (const auto NarrativeNode = Cast<UNarrativeGraphNode>(InNode))
	{
		// Base widget for Start node and any future node types
		return SNew(SNarrativeGraphNode, NarrativeNode);
	}
	
	// Return null for any non-narrative nodes - graph editor uses default
	return nullptr;
}
