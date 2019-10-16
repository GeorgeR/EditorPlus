#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EditorPlusFileLibrary.generated.h"

UCLASS()
class EDITORPLUS_API UEditorPlusFileLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    //UFUNCTION(BlueprintCallable, Category = "Editor Plus|File")
    //static UStringAssetUserData* GetOrAddStringUserData(UObject* Object);
};
