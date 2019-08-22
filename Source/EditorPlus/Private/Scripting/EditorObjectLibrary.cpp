#include "Scripting/EditorObjectLibrary.h"

void UEditorObjectLibrary::AddToRoot(UObject* Object)
{
    check(Object);

    Object->AddToRoot();
}

void UEditorObjectLibrary::RemoveFromRoot(UObject* Object)
{
    check(Object);

    Object->RemoveFromRoot();
}

template <typename ValueType>
bool UEditorObjectLibrary::SetProperty(UObject* Object, const FName& Name, ValueType Value)
{
    return false;
}