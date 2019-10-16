#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EditorPlusTextureLibrary.generated.h"

class UStaticMesh;
class UMaterialInterface;
class UTexture;
class UTexture2D;

UCLASS()
class EDITORPLUS_API UEditorPlusTextureLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Texture")
	static FLinearColor GetAverageColor(UTexture2D* Texture);
    // UFUNCTION(BlueprintCallable, Category = "Editor Plus | Texture")
    // static TArray<FString> GetMaterialSlots(UStaticMesh* Object, bool bOnlyUnset = false);
};
