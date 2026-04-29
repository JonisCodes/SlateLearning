#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"

class FNarrativeGraphNodeFactory : public FGraphPanelNodeFactory
{
public:
	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* InNode) const override;
};
