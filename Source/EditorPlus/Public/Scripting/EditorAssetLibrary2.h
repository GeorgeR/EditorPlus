#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssetData.h"

#include "EditorAssetLibrary2.generated.h"

class UStringAssetUserData;

UCLASS()
class EDITORPLUS_API UEditorAssetLibrary2
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Asset")
    static UStringAssetUserData* GetOrAddStringUserData(UObject* Object);
};