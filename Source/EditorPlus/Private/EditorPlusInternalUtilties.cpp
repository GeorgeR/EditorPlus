#include "EditorPlusInternalUtilties.h"

#include "Engine/World.h"
#include "Editor.h"

UWorld* FEditorPlusInternalUtilities::GetEditorWorld()
{
	return GEditor ? GEditor->GetEditorWorldContext(false).World() : nullptr;
}

UWorld* FEditorPlusInternalUtilities::GetEditorWorldChecked()
{
	auto* World = GetEditorWorld();
	check(World);

	return World;
}
