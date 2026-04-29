#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Styling/AppStyle.h"

class FNarrativeGraphEditorCommands : public TCommands<FNarrativeGraphEditorCommands>
{
public:
	FNarrativeGraphEditorCommands()
		: TCommands<FNarrativeGraphEditorCommands>(
		TEXT("NarrativeGraphEditor"),
		INVTEXT("Narrative Graph Editor"),
		NAME_None,
		FAppStyle::GetAppStyleSetName())
		{}
	
	virtual void RegisterCommands() override;
	
	TSharedPtr<FUICommandInfo> CompileGraph;
	TSharedPtr<FUICommandInfo> ValidateGraph;
	TSharedPtr<FUICommandInfo> ClearGraph;
	TSharedPtr<FUICommandInfo> FocusStart;
};
