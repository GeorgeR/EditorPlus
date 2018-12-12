#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EditorObjectLibrary.generated.h"

UCLASS()
class EDITORPLUS_API UEditorObjectLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Object")
    static void AddToRoot(UObject* Object);
    
    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Object")
    static void RemoveFromRoot(UObject* Object);
};