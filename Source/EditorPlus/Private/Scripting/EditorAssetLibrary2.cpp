#include "EditorAssetLibrary2.h"

#include "Interfaces/Interface_AssetUserData.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "ModuleManager.h"
#include "AssetData.h"
#include "AssetToolsModule.h"
#include "AssetRegistryModule.h"

#include "StringAssetUserData.h"

// NOTE: This doesn't seem to work
// Returns number of assets affected
TArray<FString> UEditorAssetLibrary2::GetWithCustomVersion(FGuid VersionGuid, int32 MinVersion, int32 MaxVersion)
{
	TArray<FString> AffectedAssets;
	
	auto& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	if (AssetRegistryModule.Get().IsLoadingAssets())
		return AffectedAssets;

	FARFilter Filter;
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;

	TArray<FAssetData> Assets;
	AssetRegistryModule.Get().GetAllAssets(Assets);

	if (Assets.Num() == 0)
		return AffectedAssets;

	auto& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));

	FText TaskLabel = FText::FromString(TEXT("Checking Assets for Custom Version"));
	FScopedSlowTask Task(Assets.Num(), TaskLabel);
	Task.MakeDialogDelayed(1.0f, true, false);
	
	auto AssetIndex = 0;
	for (auto& Asset : Assets)
	{
		if (Task.ShouldCancel())
			break;

		TaskLabel = FText::FromString(FString::Printf(TEXT("Checking: %s..."), *Asset.AssetName.ToString()));
		Task.EnterProgressFrame(1, TaskLabel);

		if (Asset.IsUAsset())
		{
			// #todo Only allow non-text assets
			auto Object = Asset.GetAsset();
			if (Object)
			{
				auto ObjectsCustomVersion = Object->GetLinkerCustomVersion(VersionGuid);
				auto bMatches = ObjectsCustomVersion != -1 && ObjectsCustomVersion >= MinVersion && ObjectsCustomVersion <= MaxVersion;
				if (bMatches)
				{
					AffectedAssets.Add(Asset.GetFullName());
					// #todo Unload (don't save)
				}
				else
				{
					// #todo Unload (don't save)
				}

				CollectGarbage(EObjectFlags::RF_NoFlags);
			}
		}
	}

	//Task.Destroy();
	
	return AffectedAssets;
}

int32 UEditorAssetLibrary2::ResaveWithCustomVersion(FGuid VersionGuid, int32 MinVersion, int32 MaxVersion)
{
	//AssetToolsModule.Get().Vers

	FString InputFilePath;
	auto Package = LoadPackage(nullptr, *InputFilePath, LOAD_None);
	if (Package)
	{
		FString OutputFilePath;
		UPackage::SavePackage(Package, nullptr, EObjectFlags::RF_Standalone, *OutputFilePath);
	}

	return -1;
}

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
