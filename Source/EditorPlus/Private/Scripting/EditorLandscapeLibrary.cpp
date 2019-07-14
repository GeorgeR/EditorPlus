#include "Scripting/EditorLandscapeLibrary.h"

#include "Landscape.h"
#include "LandscapeProxy.h"
#include "LandscapeInfo.h"
#include "LandscapeEdit.h"
#include "LandscapeEditorUtils.h"
#include "Foliage/Public/InstancedFoliageActor.h"
#include "LandscapeSplinesComponent.h"
#include "Editor.h"
#include "LandscapeFileFormatInterface.h"
#include "LandscapeEditorModule.h"

void UEditorLandscapeLibrary::ImportHeightmap(ALandscapeProxy* Landscape, const FString& FilePath /*= TEXT("")*/)
{
	auto ActualPath = FilePath.IsEmpty() ? Landscape->ReimportHeightmapFilePath : FilePath;
	if (ActualPath.IsEmpty())
		return;

	auto LandscapeSize = Landscape->GetBoundingRect();
	int32 NumSamplesX = LandscapeSize.Width() + 1;
	int32 NumSamplesY = LandscapeSize.Height() + 1;

	TArray<uint16> RawData;
	ReadHeightmapFile(RawData, *ActualPath, NumSamplesX, NumSamplesY);
	LandscapeEditorUtils::SetHeightmapData(Landscape, RawData);
}

void UEditorLandscapeLibrary::ImportWeightmap(ALandscapeProxy* Landscape, const FString& LayerName, const FString& FilePath /*= TEXT("")*/)
{
	FName LayerName_ = FName(*LayerName);
	for (auto& LayerSettings : Landscape->EditorLayerSettings)
	{
		if (LayerSettings.LayerInfoObj && (LayerSettings.LayerInfoObj->LayerName == LayerName_))
		{
			auto ActualPath = FilePath.IsEmpty() ? LayerSettings.ReimportLayerFilePath : FilePath;
			if (ActualPath.IsEmpty())
				return;

			auto LandscapeSize = Landscape->GetBoundingRect();
			int32 NumSamplesX = LandscapeSize.Width() + 1;
			int32 NumSamplesY = LandscapeSize.Height() + 1;

			TArray<uint8> RawData;
			ReadWeightmapFile(RawData, *ActualPath, LayerSettings.LayerInfoObj->LayerName, NumSamplesX, NumSamplesY);
			LandscapeEditorUtils::SetWeightmapData(Landscape, LayerSettings.LayerInfoObj, RawData);
		}
	}
}

void UEditorLandscapeLibrary::ReimportMaps(ALandscapeProxy* Landscape)
{
	auto LandscapeSize = Landscape->GetBoundingRect();
	int32 NumSamplesX = LandscapeSize.Width() + 1;
	int32 NumSamplesY = LandscapeSize.Height() + 1;

	if (!Landscape->ReimportHeightmapFilePath.IsEmpty() && FPaths::FileExists(Landscape->ReimportHeightmapFilePath))
	{
		TArray<uint16> RawData;
		ReadHeightmapFile(RawData, *Landscape->ReimportHeightmapFilePath, NumSamplesX, NumSamplesY);
		LandscapeEditorUtils::SetHeightmapData(Landscape, RawData);
	}

	for (auto& LayerSettings : Landscape->EditorLayerSettings)
	{
		if (LayerSettings.LayerInfoObj && FPaths::FileExists(LayerSettings.ReimportLayerFilePath))
		{
			TArray<uint8> RawData;
			ReadWeightmapFile(RawData, *LayerSettings.ReimportLayerFilePath, LayerSettings.LayerInfoObj->LayerName, NumSamplesX, NumSamplesY);
			LandscapeEditorUtils::SetWeightmapData(Landscape, LayerSettings.LayerInfoObj, RawData);
		}
	}
}

ALandscape* UEditorLandscapeLibrary::ResizeLandscape(ALandscapeProxy* Landscape, const int32 ComponentCount, const int32 SectionsPerComponent, const int32 QuadsPerSection)
{
    const int32 NewComponentSizeQuads = SectionsPerComponent * QuadsPerSection;

    auto LandscapeInfo = Landscape->GetLandscapeInfo();
    int32 OldMinX, OldMinY, OldMaxX, OldMaxY;
    if (LandscapeInfo->GetLandscapeExtent(OldMinX, OldMinY, OldMaxX, OldMaxY)) 
    {
        auto OldLandscapeProxy = LandscapeInfo->GetLandscapeProxy();

        const int32 OldVertsX = OldMaxX = OldMinX + 1;
        const int32 OldVertsY = OldMaxY - OldMinY + 1;
        const int32 NewVertsX = ComponentCount * NewComponentSizeQuads + 1;
        const int32 NewVertsY = ComponentCount * NewComponentSizeQuads + 1;

        FLandscapeEditDataInterface LandscapeEdit(LandscapeInfo);
        TArray<uint16> HeightData;
        TArray<FLandscapeImportLayerInfo> ImportLayerInfos;
        FVector LandscapeOffset = FVector::ZeroVector;
        FIntPoint LandscapeOffsetQuads = FIntPoint::ZeroValue;
        float LandscapeScaleFactor = 1.0f;

        int32 NewMinX, NewMinY, NewMaxX, NewMaxY;
        NewMinX = OldMinX / LandscapeInfo->ComponentSizeQuads * NewComponentSizeQuads;
        NewMinY = OldMinY / LandscapeInfo->ComponentSizeQuads * NewComponentSizeQuads;
        NewMaxX = NewMinX + NewVertsX - 1;
        NewMaxY = NewMinY + NewVertsY - 1;

        HeightData.AddZeroed(OldVertsX * OldVertsY * sizeof(uint16));

        int32 TMinX = OldMinX, TMinY = OldMinY, TMaxX = OldMaxX, TMaxY = OldMaxY;
        LandscapeEdit.GetHeightData(TMinX, TMinY, TMaxX, TMaxY, HeightData.GetData(), 0);

        HeightData = LandscapeEditorUtils::ResampleData(HeightData, OldVertsX, OldVertsY, NewVertsX, NewVertsY);

        for (const FLandscapeInfoLayerSettings& LayerSettings : LandscapeInfo->Layers)
        {
            if (LayerSettings.LayerInfoObj != nullptr)
            {
                auto ImportLayerInfo = new(ImportLayerInfos)FLandscapeImportLayerInfo(LayerSettings);
                ImportLayerInfo->LayerData.AddZeroed(OldVertsX * OldVertsY * sizeof(uint8));

                TMinX = OldMinX; TMinY = OldMinY; TMaxX = OldMaxX; TMaxY = OldMaxY;
                LandscapeEdit.GetWeightData(LayerSettings.LayerInfoObj, TMinX, TMinY, TMaxX, TMaxY, ImportLayerInfo->LayerData.GetData(), 0);

                ImportLayerInfo->LayerData = LandscapeEditorUtils::ResampleData(ImportLayerInfo->LayerData, OldVertsX, OldVertsY, NewVertsX, NewVertsY);
            }
        }

        LandscapeScaleFactor = (float)OldLandscapeProxy->ComponentSizeQuads / NewComponentSizeQuads;

        const FVector Location = OldLandscapeProxy->GetActorLocation() + LandscapeOffset;
        FActorSpawnParameters SpawnParams;
        SpawnParams.OverrideLevel = OldLandscapeProxy->GetLevel();
        auto NewLandscape = Cast<ALandscape>(OldLandscapeProxy->GetWorld()->SpawnActor<ALandscape>(Location, OldLandscapeProxy->GetActorRotation(), SpawnParams));

        const FVector OldScale = OldLandscapeProxy->GetActorScale();
        NewLandscape->SetActorRelativeScale3D(FVector(OldScale.X * LandscapeScaleFactor, OldScale.Y * LandscapeScaleFactor, OldScale.Z));

        NewLandscape->LandscapeMaterial = OldLandscapeProxy->LandscapeMaterial;
        NewLandscape->LandscapeMaterialsOverride = OldLandscapeProxy->LandscapeMaterialsOverride;
        NewLandscape->CollisionMipLevel = OldLandscapeProxy->CollisionMipLevel;
        //NewLandscape->Import(FGuid::NewGuid(), NewMinX, NewMinY, NewMaxX, NewMaxY, SectionsPerComponent, QuadsPerSection, HeightData.GetData(), *OldLandscapeProxy->ReimportHeightmapFilePath, ImportLayerInfos, ELandscapeImportAlphamapType::Additive);

        NewLandscape->MaxLODLevel = OldLandscapeProxy->MaxLODLevel;
        NewLandscape->LODDistanceFactor_DEPRECATED = OldLandscapeProxy->LODDistanceFactor_DEPRECATED;
        NewLandscape->LODFalloff_DEPRECATED = OldLandscapeProxy->LODFalloff_DEPRECATED;
        NewLandscape->TessellationComponentScreenSize = OldLandscapeProxy->TessellationComponentScreenSize;
        NewLandscape->ComponentScreenSizeToUseSubSections = OldLandscapeProxy->ComponentScreenSizeToUseSubSections;
        NewLandscape->UseTessellationComponentScreenSizeFalloff = OldLandscapeProxy->UseTessellationComponentScreenSizeFalloff;
        NewLandscape->TessellationComponentScreenSizeFalloff = OldLandscapeProxy->TessellationComponentScreenSizeFalloff;
        NewLandscape->LODDistributionSetting = OldLandscapeProxy->LODDistributionSetting;
        NewLandscape->LOD0DistributionSetting = OldLandscapeProxy->LOD0DistributionSetting;
        NewLandscape->OccluderGeometryLOD = OldLandscapeProxy->OccluderGeometryLOD;
        NewLandscape->ExportLOD = OldLandscapeProxy->ExportLOD;
        NewLandscape->StaticLightingLOD = OldLandscapeProxy->StaticLightingLOD;
        NewLandscape->NegativeZBoundsExtension = OldLandscapeProxy->NegativeZBoundsExtension;
        NewLandscape->PositiveZBoundsExtension = OldLandscapeProxy->PositiveZBoundsExtension;
        NewLandscape->DefaultPhysMaterial = OldLandscapeProxy->DefaultPhysMaterial;
        NewLandscape->StreamingDistanceMultiplier = OldLandscapeProxy->StreamingDistanceMultiplier;
        NewLandscape->LandscapeHoleMaterial = OldLandscapeProxy->LandscapeHoleMaterial;
        NewLandscape->StaticLightingResolution = OldLandscapeProxy->StaticLightingResolution;
        NewLandscape->bCastStaticShadow = OldLandscapeProxy->bCastStaticShadow;
        NewLandscape->bCastShadowAsTwoSided = OldLandscapeProxy->bCastShadowAsTwoSided;
        NewLandscape->LightingChannels = OldLandscapeProxy->LightingChannels;
        NewLandscape->bRenderCustomDepth = OldLandscapeProxy->bRenderCustomDepth;
        NewLandscape->CustomDepthStencilValue = OldLandscapeProxy->CustomDepthStencilValue;
        NewLandscape->LightmassSettings = OldLandscapeProxy->LightmassSettings;
        NewLandscape->CollisionThickness = OldLandscapeProxy->CollisionThickness;
        NewLandscape->BodyInstance.SetCollisionProfileName(OldLandscapeProxy->BodyInstance.GetCollisionProfileName());

        if (NewLandscape->BodyInstance.DoesUseCollisionProfile() == false)
        {
            NewLandscape->BodyInstance.SetCollisionEnabled(OldLandscapeProxy->BodyInstance.GetCollisionEnabled());
            NewLandscape->BodyInstance.SetObjectType(OldLandscapeProxy->BodyInstance.GetObjectType());
            NewLandscape->BodyInstance.SetResponseToChannels(OldLandscapeProxy->BodyInstance.GetResponseToChannels());
        }

        NewLandscape->EditorLayerSettings = OldLandscapeProxy->EditorLayerSettings;
        NewLandscape->bUsedForNavigation = OldLandscapeProxy->bUsedForNavigation;
        NewLandscape->MaxPaintedLayersPerComponent = OldLandscapeProxy->MaxPaintedLayersPerComponent;

        NewLandscape->CreateLandscapeInfo();

        // Clone landscape splines
        TLazyObjectPtr<ALandscape> OldLandscapeActor = LandscapeInfo->LandscapeActor;
        if (OldLandscapeActor.IsValid() && OldLandscapeActor->SplineComponent != NULL)
        {
            auto OldSplines = OldLandscapeActor->SplineComponent;
            auto NewSplines = DuplicateObject<ULandscapeSplinesComponent>(OldSplines, NewLandscape, OldSplines->GetFName());
            NewSplines->AttachToComponent(NewLandscape->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

            const FVector OldSplineScale = OldSplines->GetRelativeTransform().GetScale3D();
            NewSplines->SetRelativeScale3D(FVector(OldSplineScale.X / LandscapeScaleFactor, OldSplineScale.Y / LandscapeScaleFactor, OldSplineScale.Z));
            NewLandscape->SplineComponent = NewSplines;
            NewSplines->RegisterComponent();
        }

        auto NewLandscapeInfo = NewLandscape->GetLandscapeInfo();
        for (const TPair<FIntPoint, ULandscapeComponent*>& Entry : LandscapeInfo->XYtoComponentMap)
        {
            auto NewComponent = NewLandscapeInfo->XYtoComponentMap.FindRef(Entry.Key);
            if (NewComponent)
            {
                auto OldCollisionComponent = Entry.Value->CollisionComponent.Get();
                auto NewCollisionComponent = NewComponent->CollisionComponent.Get();

                if (OldCollisionComponent && NewCollisionComponent)
                {
                    AInstancedFoliageActor::MoveInstancesToNewComponent(OldCollisionComponent->GetWorld(), OldCollisionComponent, NewCollisionComponent);
                    NewCollisionComponent->SnapFoliageInstances(FBox(FVector(-WORLD_MAX), FVector(WORLD_MAX)));
                }
            }
        }

        // delete any components that were deleted in the original
        TSet<ULandscapeComponent*> ComponentsToDelete;
        for (const TPair<FIntPoint, ULandscapeComponent*>& Entry : NewLandscapeInfo->XYtoComponentMap)
        {
            if (!LandscapeInfo->XYtoComponentMap.Contains(Entry.Key))
                ComponentsToDelete.Add(Entry.Value);
        }

        // TODO
        //if (ComponentsToDelete.Num() > 0)
        //    FEdModeLandscape::DeleteLandscapeComponents(NewLandscapeInfo, ComponentsToDelete);

        GEditor->RedrawLevelEditingViewports();

        return NewLandscape;
    }

    return nullptr;
}

void UEditorLandscapeLibrary::SetLocationZ(ALandscapeProxy* Landscape, const float LocationZ)
{

}

bool UEditorLandscapeLibrary::ReadHeightmapFile(TArray<uint16>& Result, const FString& Filename, int32 ExpectedWidth, int32 ExpectedHeight)
{
	bool bResult = true;

	ILandscapeEditorModule& LandscapeEditorModule = FModuleManager::GetModuleChecked<ILandscapeEditorModule>("LandscapeEditor");
	const ILandscapeHeightmapFileFormat* HeightmapFormat = LandscapeEditorModule.GetHeightmapFormatByExtension(*FPaths::GetExtension(Filename, true));

	FLandscapeHeightmapImportData ImportData = HeightmapFormat->Import(*Filename, { (uint32)ExpectedWidth, (uint32)ExpectedHeight });
	if (ImportData.ResultCode != ELandscapeImportResult::Error)
	{
		Result = MoveTemp(ImportData.Data);
	}
	else
	{
		UE_LOG(LogStreaming, Warning, TEXT("%s"), *ImportData.ErrorMessage.ToString());
		Result.Empty();
		bResult = false;
	}

	return bResult;
}

bool UEditorLandscapeLibrary::ReadWeightmapFile(TArray<uint8>& Result, const FString& Filename, FName LayerName, int32 ExpectedWidth, int32 ExpectedHeight)
{
	bool bResult = true;

	ILandscapeEditorModule& LandscapeEditorModule = FModuleManager::GetModuleChecked<ILandscapeEditorModule>("LandscapeEditor");
	const ILandscapeWeightmapFileFormat* WeightmapFormat = LandscapeEditorModule.GetWeightmapFormatByExtension(*FPaths::GetExtension(Filename, true));

	FLandscapeWeightmapImportData ImportData = WeightmapFormat->Import(*Filename, LayerName, { (uint32)ExpectedWidth, (uint32)ExpectedHeight });
	if (ImportData.ResultCode != ELandscapeImportResult::Error)
	{
		Result = MoveTemp(ImportData.Data);
	}
	else
	{
		UE_LOG(LogStreaming, Warning, TEXT("%s"), *ImportData.ErrorMessage.ToString());
		Result.Empty();
		bResult = false;
	}

	return bResult;
}

void UEditorLandscapeLibrary::SetLevelPosition(const FIntVector& Position)
{

}
