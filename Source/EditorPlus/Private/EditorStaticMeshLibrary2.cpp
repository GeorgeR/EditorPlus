#include "EditorStaticMeshLibrary2.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"

TArray<FString> UEditorStaticMeshLibrary2::GetMaterialSlots(UStaticMesh* Object)
{
    check(Object);

    TArray<FString> Result;
    for (auto& Material : Object->StaticMaterials)
        Result.Add(Material.MaterialSlotName.ToString());
    
    return Result;
}

void UEditorStaticMeshLibrary2::SetMaterialSlot_ByName(UStaticMesh* Object, const FString& Name, UMaterialInterface* Material)
{
    check(Object);

    FName Key = *Name;
    auto Index = Object->StaticMaterials.IndexOfByPredicate([Key](const FStaticMaterial& Mtl) { return Mtl.MaterialSlotName == Key; });
    if (Index == INDEX_NONE)
        return;

    SetMaterialSlot_ByIndex(Object, Index, Material);
}

void UEditorStaticMeshLibrary2::SetMaterialSlot_ByIndex(UStaticMesh* Object, const int32 Index, UMaterialInterface* Material)
{
    check(Object);

    Object->SetMaterial(Index, Material);
}