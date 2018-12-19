#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssetData.h"

#include "EditorStaticMeshLibrary2.generated.h"

class UStaticMesh;

UCLASS()
class EDITORPLUS_API UEditorStaticMeshLibrary2
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | StaticMesh")
    static TArray<FString> GetMaterialSlots(UStaticMesh* Object);

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | StaticMesh", meta = (DisplayName = "SetMaterialSlot"))
    static void SetMaterialSlot_ByName(UStaticMesh* Object, const FString& Name, UMaterialInterface* Material);

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | StaticMesh", meta = (DisplayName = "SetMaterialSlot"))
    static void SetMaterialSlot_ByIndex(UStaticMesh* Object, const int32 Index, UMaterialInterface* Material);

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | StaticMesh")
    static void SetMeshBuildSettings(UStaticMesh* Object, bool bRecomputeTangents, bool bRecomputeNormals);
};