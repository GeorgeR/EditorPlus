#include "Scripting/EditorPlusMaterialLibrary.h"

#include "Materials/MaterialInterface.h"

#include "MaterialUtilities/Public/MaterialUtilities.h"
#include "Materials/MaterialInstanceConstant.h"
#include "AssetRegistryModule.h"
#include "ModuleManager.h"
#include "ObjectTools.h"
#include "Tests/AutomationEditorCommon.h"
#include "Engine/Texture.h"
#include "MaterialEditingLibrary.h"

UMaterialInterface* UEditorPlusMaterialLibrary::CreateMaterialInstance_FromPath(const FString& BaseMaterialPath, const FString& DestinationPath)
{
    auto BaseMaterial = LoadObject<UMaterialInterface>(nullptr, *BaseMaterialPath);
    return CreateMaterialInstance(BaseMaterial, DestinationPath);
}

UMaterialInterface* UEditorPlusMaterialLibrary::CreateMaterialInstance(UMaterialInterface* BaseMaterial, const FString& DestinationPath)
{
    //FAssetRegistryModule& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
    //FAssetData AssetData = AssetRegistry.Get().GetAssetByObjectPath(FName(*DestinationPath));
    //if (AssetData.IsValid())
    //{
    //    TArray<UObject*> ExistingObject = { LoadObject<UObject>(nullptr, *DestinationPath) };
    //    ObjectTools::ForceDeleteObjects(ExistingObject, false);
    //}

    const FString AssetBaseName = FPackageName::GetShortName(DestinationPath);
    const FString AssetBasePath = FPackageName::GetLongPackagePath(DestinationPath) + TEXT("/");

    auto MaterialOuter = CreatePackage(nullptr, *(AssetBasePath + AssetBaseName));
    MaterialOuter->FullyLoad();
    MaterialOuter->Modify();

    auto ExistingPackage = FindObject<UMaterial>(MaterialOuter, *AssetBaseName);
    if (ExistingPackage && !ExistingPackage->IsA<UMaterialInstanceConstant>())
    {
#if WITH_EDITOR
        FAutomationEditorCommonUtils::NullReferencesToObject(ExistingPackage);
#endif
        ExistingPackage->MarkPendingKill();
        CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS, true);
    }

    UMaterialInstanceConstant* MaterialInstance = NewObject<UMaterialInstanceConstant>(MaterialOuter, FName(*AssetBaseName), EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);
    MaterialInstance->Parent = BaseMaterial;

    return MaterialInstance;
}

bool UEditorPlusMaterialLibrary::SetVectorParameter(UMaterialInterface* Material, const FName Name, FLinearColor Value)
{
    check(Material);

    auto MaterialInstance = Cast<UMaterialInstance>(Material);
    if (MaterialInstance != nullptr)
    {
        auto MaterialInstanceConstant = Cast<UMaterialInstanceConstant>(MaterialInstance);
        if (MaterialInstanceConstant != nullptr)
        {
            return UMaterialEditingLibrary::SetMaterialInstanceVectorParameterValue(MaterialInstanceConstant, Name, Value);
        }
    }

    return false;
}

bool UEditorPlusMaterialLibrary::SetVectorParameter_FromVector(UMaterialInterface* Material, const FName Name, FVector Value)
{
    return SetVectorParameter(Material, Name, Value);
}

bool UEditorPlusMaterialLibrary::SetScalarParameter(UMaterialInterface* Material, const FName Name, float Value)
{
    check(Material);

    auto MaterialInstance = Cast<UMaterialInstance>(Material);
    if (MaterialInstance != nullptr)
    {
        auto MaterialInstanceConstant = Cast<UMaterialInstanceConstant>(MaterialInstance);
        if (MaterialInstanceConstant != nullptr)
        {
            return UMaterialEditingLibrary::SetMaterialInstanceScalarParameterValue(MaterialInstanceConstant, Name, Value);
        }
    }

    return false;
}

bool UEditorPlusMaterialLibrary::SetTextureParameter(UMaterialInterface* Material, const FName Name, UTexture* Value)
{
    check(Material);
    check(Value);

    auto MaterialInstance = Cast<UMaterialInstance>(Material);
    if (MaterialInstance != nullptr)
    {
        auto MaterialInstanceConstant = Cast<UMaterialInstanceConstant>(MaterialInstance);
        if (MaterialInstanceConstant != nullptr)
        {
            return UMaterialEditingLibrary::SetMaterialInstanceTextureParameterValue(MaterialInstanceConstant, Name, Value);
        }
    }

    return false;
}