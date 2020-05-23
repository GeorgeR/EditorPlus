#include "Scripting/EditorPlusObjectLibrary.h"

void UEditorPlusObjectLibrary::AddToRoot(UObject* Object)
{
    check(Object);

    Object->AddToRoot();
}

void UEditorPlusObjectLibrary::RemoveFromRoot(UObject* Object)
{
    check(Object);

    Object->RemoveFromRoot();
}
