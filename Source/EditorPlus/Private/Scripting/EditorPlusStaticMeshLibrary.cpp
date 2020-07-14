#include "Scripting/EditorPlusStaticMeshLibrary.h"

#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "MeshDescription.h"

#if ENGINE_MINOR_VERSION >= 23
#include "StaticMeshAttributes.h"
#endif

TArray<FString> UEditorPlusStaticMeshLibrary::GetMaterialSlots(UStaticMesh* Object, bool bOnlyUnset)
{
    check(Object);

    TArray<FString> Result;
    for (auto& Material : Object->StaticMaterials)
        if(bOnlyUnset && Material.MaterialInterface == nullptr)
            Result.Add(Material.MaterialSlotName.ToString());

    return Result;
}

void UEditorPlusStaticMeshLibrary::SetMaterialSlot_ByName(UStaticMesh* Object, const FString& Name, UMaterialInterface* Material)
{
    check(Object);

    FName Key = *Name;
    auto Index = Object->StaticMaterials.IndexOfByPredicate([Key](const FStaticMaterial& Mtl) { return Mtl.MaterialSlotName == Key; });
    if (Index == INDEX_NONE)
        return;

    SetMaterialSlot_ByIndex(Object, Index, Material);
}

void UEditorPlusStaticMeshLibrary::SetMaterialSlot_ByIndex(UStaticMesh* Object, const int32 Index, UMaterialInterface* Material)
{
    check(Object);

    Object->SetMaterial(Index, Material);
}

void UEditorPlusStaticMeshLibrary::SetMeshBuildSettings(UStaticMesh* Object, bool bRecomputeTangents, bool bRecomputeNormals)
{
    check(Object);

    auto NumLODs = Object->GetNumLODs();
    for (auto i = 0; i < NumLODs; i++)
    {
#if ENGINE_MINOR_VERSION <= 22
        FStaticMeshSourceModel& SourceModel = Object->SourceModels[i];
#else
        FStaticMeshSourceModel& SourceModel = Object->GetSourceModel(i);
#endif
        SourceModel.BuildSettings.bRecomputeNormals = bRecomputeNormals;
        SourceModel.BuildSettings.bRecomputeTangents = bRecomputeTangents;
    }

    Object->Build(false);
    Object->PostEditChange();
    Object->MarkPackageDirty();
}

void UEditorPlusStaticMeshLibrary::SetUVConstant(UStaticMesh* Object, const uint8& Channel, const FVector2D& UV)
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

bool UEditorPlusStaticMeshLibrary::GetUniformOrPerVertexColors(UStaticMesh* Object, TArray<FColor>& OutColors, FColor& OutColor, const int32 LODIndex)
{
	check(Object);

#if ENGINE_MINOR_VERSION >= 22
    const auto MeshDescription = Object->GetMeshDescription(LODIndex);
#else
    const auto MeshDescription = Object->GetOriginalMeshDescription(LODIndex);
#endif

	bool bHasColors = false;
#if ENGINE_MINOR_VERSION >= 23
	bHasColors = MeshDescription->VertexInstanceAttributes().HasAttribute(MeshAttribute::VertexInstance::Color);
#else
		// @todo
#endif

	if(!bHasColors)
		return false;

#if ENGINE_MINOR_VERSION >= 23
    const TVertexInstanceAttributesConstRef<FVector4> VertexInstanceColors = MeshDescription->VertexInstanceAttributes().GetAttributesRef<FVector4>(MeshAttribute::VertexInstance::Color);
	OutColors.SetNum(VertexInstanceColors.GetNumElements());
	if(OutColors.Num() <= 0)
		return false;

	for(const auto& VertexInstanceID : MeshDescription->VertexInstances().GetElementIDs())
	{
		const auto& VertexInstanceColor = VertexInstanceColors[VertexInstanceID];
		const FLinearColor LinearColor(VertexInstanceColor);
		OutColors.Emplace(LinearColor.ToFColor(true));
	    //OutColors[VertexInstanceID.GetValue()] = ;
	}
	OutColor = OutColors[0];
#else
	// @todo
#endif

	return OutColors.Num() > 0;
}
