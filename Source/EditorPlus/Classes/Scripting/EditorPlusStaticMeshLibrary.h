#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EditorPlusStaticMeshLibrary.generated.h"

class UStaticMesh;
class UMaterialInterface;

UCLASS()
class EDITORPLUS_API UEditorPlusStaticMeshLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Editor Plus | StaticMesh")
    static TArray<FString> GetMaterialSlots(UStaticMesh* Object, bool bOnlyUnset = false);

    UFUNCTION(BlueprintCallable, Category = "Editor Plus | StaticMesh", meta = (DisplayName = "Set Material Slot (by name)"))
    static void SetMaterialSlot_ByName(UStaticMesh* Object, const FString& Name, UMaterialInterface* Material);

    UFUNCTION(BlueprintCallable, Category = "Editor Plus | StaticMesh", meta = (DisplayName = "Set Material Slot (by index)"))
    static void SetMaterialSlot_ByIndex(UStaticMesh* Object, const int32 Index, UMaterialInterface* Material);

    UFUNCTION(BlueprintCallable, Category = "Editor Plus | StaticMesh")
    static void SetMeshBuildSettings(UStaticMesh* Object, bool bRecomputeTangents, bool bRecomputeNormals);

    /* For encoding an entire mesh as a "pixel" in a UV */
    UFUNCTION(BlueprintCallable, Category = "Editor Plus | StaticMesh")
    static void SetUVConstant(UStaticMesh* Object, const uint8& Channel, const FVector2D& UV);
};
