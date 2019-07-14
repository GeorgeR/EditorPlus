#include "Scripting/EditorStaticMeshLibrary2.h"

#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "MeshDescription.h"

TArray<FString> UEditorStaticMeshLibrary2::GetMaterialSlots(UStaticMesh* Object, bool bOnlyUnset)
{
    check(Object);

    TArray<FString> Result;
    for (auto& Material : Object->StaticMaterials)
        if(bOnlyUnset && Material.MaterialInterface == nullptr)
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

void UEditorStaticMeshLibrary2::SetMeshBuildSettings(UStaticMesh* Object, bool bRecomputeTangents, bool bRecomputeNormals)
{
    check(Object);

    auto NumLODs = Object->GetNumLODs();
    for (auto i = 0; i < NumLODs; i++)
    {
        FStaticMeshSourceModel& SourceModel = Object->SourceModels[i];
        SourceModel.BuildSettings.bRecomputeNormals = bRecomputeNormals;
        SourceModel.BuildSettings.bRecomputeTangents = bRecomputeTangents;
    }

    Object->Build(false);
    Object->PostEditChange();
    Object->MarkPackageDirty();
}

void UEditorStaticMeshLibrary2::SetUVConstant(UStaticMesh* Object, const uint8& Channel, const FVector2D& UV)
{
    check(Object);

    auto LODCount = Object->GetNumLODs();
    for (auto LODIndex = 0; LODIndex < LODCount; LODIndex++)
    {
        if (Object->GetNumUVChannels(LODIndex) - 1 != Channel)
        {
            auto bChannelWasAdded = Object->InsertUVChannel(LODIndex, Channel);
            if (!bChannelWasAdded)
            {
                while (Object->GetNumUVChannels(LODIndex) - 1 < Channel)
                    bChannelWasAdded = Object->AddUVChannel(LODIndex);
            }
        }

#if ENGINE_MINOR_VERSION >= 22
        auto MeshDescription = Object->GetMeshDescription(LODIndex);
#else
        auto MeshDescription = Object->GetOriginalMeshDescription(LODIndex);
#endif

#if ENGINE_MINOR_VERSION >= 23
		TMap<FVertexInstanceID, FVector2D> TexCoords;
		TexCoords.Reserve(MeshDescription->VertexInstances().Num());
#else
		TArray<FVector2D> TexCoords;
		TexCoords.Init(UV, MeshDescription->VertexInstances().Num());
#endif
        
        Object->SetUVChannel(LODIndex, Channel, TexCoords);
    }
}
