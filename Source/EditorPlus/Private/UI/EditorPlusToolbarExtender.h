#pragma once

#include "CoreMinimal.h"
#include "LevelEditor.h"

class EDITORPLUS_API FEditorPlusToolbarExtender
{
public:
	void Extend();
	void Release();

private:
	FDelegateHandle LevelViewportExtenderHandle;
	TSharedPtr<class FExtender> LevelToolbarExtender;
};