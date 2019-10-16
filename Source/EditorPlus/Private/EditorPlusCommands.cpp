#include "EditorPlusCommands.h"

#include "Framework/Commands/Commands.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/Commands/UICommandList.h"
#include "UObject/Object.h"

#include "Scripting/EditorPlusUtilityLibrary.h"

#define LOCTEXT_NAMESPACE "EditorPlus"

FEditorPlusCommands::FEditorPlusCommands()
    : TCommands<FEditorPlusCommands>(TEXT("EditorPlus"),
    NSLOCTEXT("Contexts", "EditorPlus", "EditorPlus"),
    NAME_None,
    FEditorPlusCommands::GetStyleSetName()) { }

void FEditorPlusCommands::RegisterCommands()
{
	UI_COMMAND(FreeMemoryCommand, "Free Memory (GC)", "Frees memory via garbage collection.", EUserInterfaceActionType::Button, FInputChord(EKeys::Enter));

    LevelMenuActionList = MakeShareable(new FUICommandList);
	LevelMenuActionList->MapAction(
		FreeMemoryCommand,
		FExecuteAction::CreateStatic(&UEditorPlusUtilityLibrary::CollectGarbage),
		FCanExecuteAction());
}

#undef LOCTEXT_NAMESPACE