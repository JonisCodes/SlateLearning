#include "FNarrativeGraphEditorCommands.h"

#define LOCTEXT_NAMESPACE "NarrativeGraphEditorCommands"

void FNarrativeGraphEditorCommands::RegisterCommands()
{
	UI_COMMAND(CompileGraph,
		"Compile",
		"Compile the narrative graph",
		EUserInterfaceActionType::Button,
		FInputChord());
	
	UI_COMMAND(ValidateGraph,
		"Validate",
		"Check the graph for errors",
		EUserInterfaceActionType::Button,
		FInputChord());
	
	UI_COMMAND(ClearGraph,
		"Clear",
		"Remove all nodes from the graph",
		EUserInterfaceActionType::Button,
		FInputChord());
	
	UI_COMMAND(FocusStart,
		"Focus Start",
		"Focus the viewport on the Start node",
		EUserInterfaceActionType::Button,
		FInputChord(EKeys::Home));
}

#undef LOCTEXT_NAMESPACE