#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Framework/Commands/UICommandInfo.h"

#include "EditorPlusStyle.h"

class FUICommandList;

class EDITORPLUS_API FEditorPlusCommands 
    : public TCommands<FEditorPlusCommands>
{
public:
    FEditorPlusCommands();

	void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> FreeMemoryCommand;
    TSharedPtr<FUICommandList> LevelMenuActionList;
};