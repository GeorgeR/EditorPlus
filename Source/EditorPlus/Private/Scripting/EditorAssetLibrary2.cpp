#include "Scripting/EditorAssetLibrary2.h"

#include "Interfaces/Interface_AssetUserData.h"
#include "StringAssetUserData.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "ModuleManager.h"
#include "AssetData.h"
#include "AssetToolsModule.h"

UStringAssetUserData* UEditorAssetLibrary2::GetOrAddStringUserData(UObject* Object)
{
    check(Object);

    if (IInterface_AssetUserData* AssetUserData = Cast<IInterface_AssetUserData>(Object))
    {
        auto Result = Cast<UStringAssetUserData>(AssetUserData->GetAssetUserDataOfClass(UStringAssetUserData::StaticClass()));
        if (Result == nullptr)
        {
            Result = NewObject<UStringAssetUserData>();
            AssetUserData->AddAssetUserData(Result);
        }

        return Result;
    }

    return nullptr;
}

TArray<UObject*> UEditorAssetLibrary2::GetSelected()
{
	auto& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	TArray<FAssetData> SelectedAssets;
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);

	TArray<UObject*> Result;
	for (auto& AssetData : SelectedAssets)
		Result.Add(AssetData.GetAsset());

	return Result;
}

void UEditorAssetLibrary2::RenameAsset(UObject* Asset, const FString& NewName)
{
	auto& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));

	TArray<FAssetRenameData> AssetsAndNames;
	const FString PackagePath = FPackageName::GetLongPackagePath(Asset->GetOutermost()->GetName());
	new (AssetsAndNames) FAssetRenameData(Asset, PackagePath, NewName);

	AssetToolsModule.Get().RenameAssetsWithDialog(AssetsAndNames);
}
