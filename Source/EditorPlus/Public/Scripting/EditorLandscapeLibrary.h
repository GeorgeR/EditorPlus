#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssetData.h"

#include "EditorLandscapeLibrary.generated.h"

class ALandscape;
class ALandscapeProxy;

UCLASS()
class EDITORPLUS_API UEditorLandscapeLibrary 
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Landscape")
    static void ImportHeightmap(ALandscapeProxy* Landscape, const FString& FilePath, bool bResize = true);

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Landscape")
    static ALandscape* ResizeLandscape(ALandscapeProxy* Landscape, const int32 ComponentCount, const int32 SectionsPerComponent, const int32 QuadsPerSection);
};