#include "Scripting/EditorPlusMaterialLibrary.h"

#include "Materials/MaterialInterface.h"

#include "MaterialUtilities/Public/MaterialUtilities.h"
#include "Materials/MaterialInstanceConstant.h"
#include "AssetRegistryModule.h"
#include "Modules/ModuleManager.h"
#include "ObjectTools.h"
#include "Tests/AutomationEditorCommon.h"
#include "Engine/Texture.h"
#include "MaterialEditingLibrary.h"

UMaterialInterface* UEditorPlusMaterialLibrary::CreateMaterialInstance_FromPath(const FString& BaseMaterialPath, const FString& DestinationPath)
{
	const auto BaseMaterial = LoadObject<UMaterialInterface>(nullptr, *BaseMaterialPath);
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

    const auto AssetBaseName = FPackageName::GetShortName(DestinationPath);
    const auto AssetBasePath = FPackageName::GetLongPackagePath(DestinationPath) + TEXT("/");

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

    const auto MaterialInstance = NewObject<UMaterialInstanceConstant>(MaterialOuter, FName(*AssetBaseName), EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);
    MaterialInstance->Parent = BaseMaterial;

    return MaterialInstance;
}

bool UEditorPlusMaterialLibrary::SetVectorParameter(UMaterialInterface* Material, const FName Name, FLinearColor Value)
{
        {
			return SetMaterialInstanceParameter<FLinearColor>(Material, Name, Value, UMaterialEditingLibrary::SetMaterialInstanceVectorParameterValue);
		}

bool UEditorPlusMaterialLibrary::SetVectorParameter_FromVector(UMaterialInterface* Material, const FName Name, FVector Value)
{
    return SetVectorParameter(Material, Name, Value);
}

bool UEditorPlusMaterialLibrary::SetScalarParameter(UMaterialInterface* Material, const FName Name, float Value)
{
    return SetMaterialInstanceParameter<float>(Material, Name, Value, UMaterialEditingLibrary::SetMaterialInstanceScalarParameterValue);
}

bool UEditorPlusMaterialLibrary::SetTextureParameter(UMaterialInterface* Material, const FName Name, UTexture* Value)
{

	check(Material);    check(Value);

    return SetMaterialInstanceParameter<UTexture*>(Material, Name, Value, UMaterialEditingLibrary::SetMaterialInstanceTextureParameterValue);
}

FLinearColor UEditorPlusMaterialLibrary::GetAverageBaseColor(UMaterialInterface* Material)
{
	check(Material);

	// @todo

	return FLinearColor::Transparent;
}

template <typename T>
bool UEditorPlusMaterialLibrary::SetMaterialInstanceParameter(
    UMaterialInterface* Material, const FName Name, T Value,
    TFunctionRef<bool(UMaterialInstanceConstant*, FName, T)> Func)
{
    const auto MaterialInstance = Cast<UMaterialInstance>(Material);
    if (MaterialInstance != nullptr)
    {
	    const auto MaterialInstanceConstant = Cast<UMaterialInstanceConstant>(MaterialInstance);
        if (MaterialInstanceConstant != nullptr)
        {
            FMaterialUpdateContext UpdateContext(FMaterialUpdateContext::EOptions::SyncWithRenderingThread);
            UpdateContext.AddMaterialInstance(MaterialInstanceConstant);

            return Func(MaterialInstanceConstant, Name, Value);
        }
    }

    return false;
}
}