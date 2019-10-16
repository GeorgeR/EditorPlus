#include "Scripting/EditorPlusUtilityLibrary.h"

#include "Editor.h"
#include "EditorPlusInternalUtilties.h"

void UEditorPlusUtilityLibrary::CollectGarbage()
{
    const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();

    //World->ForceGarbageCollection(true);
	GEditor->ForceGarbageCollection(true);
}

void UEditorPlusUtilityLibrary::Sleep(float Seconds)
{
	FPlatformProcess::Sleep(Seconds);
}
