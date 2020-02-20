#include "Scripting/EditorPlusAssetLibrary.h"

#include "Interfaces/Interface_AssetUserData.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "ModuleManager.h"
#include "AssetData.h"
#include "AssetToolsModule.h"
#include "AssetRegistryModule.h"
#include "FileHelpers.h"
#include "IAssetTools.h"
#include "PackageTools.h"

#include "StringAssetUserData.h"

bool UEditorPlusAssetLibrary::SplitObjectPath_Object(
	UObject* Object, 
	FString& OutPath, 
	FString& OutName, 
	FString& OutClassName, 
	FString& OutExtension)
{
	check(Object);

	if(!Object->IsAsset())
		return false;
	
	return SplitObjectPath_ObjectPath(Object->GetPathName(), OutPath, OutName, OutClassName, OutExtension);
}

bool UEditorPlusAssetLibrary::SplitObjectPath_Name(
	const FName& Path, 
	FString& OutPath, 
	FString& OutName, 
	FString& OutClassName, 
	FString& OutExtension)
{
	return SplitObjectPath_ObjectPath(Path.ToString(), OutPath, OutName, OutClassName, OutExtension);
}

bool UEditorPlusAssetLibrary::SplitObjectPath_AssetData(
	const FAssetData& AssetData, 
	FString& OutPath, 
	FString& OutName, 
	FString& OutClassName, 
	FString& OutExtension)
{
	check(AssetData.IsValid());
	
	return SplitObjectPath_ObjectPath(AssetData.ToSoftObjectPath(), OutPath, OutName, OutClassName, OutExtension);
}

bool UEditorPlusAssetLibrary::SplitObjectPath_ObjectPath(
	const FSoftObjectPath& ObjectPath, 
	FString& OutPath, 
	FString& OutName, 
	FString& OutClassName, 
	FString& OutExtension)
{
	check(ObjectPath.IsValid());

	const auto PathName = ObjectPath.GetAssetPathString();
	return SplitObjectPath_String(PathName, OutPath, OutName, OutClassName, OutExtension);
}

bool UEditorPlusAssetLibrary::SplitObjectPath_String(
	const FString& Path, 
	FString& OutPath, 
	FString& OutName, 
	FString& OutClassName, 
	FString& OutExtension)
{
	auto WorkingPath = Path;

	auto FirstSlashIndex = -1;
	if(WorkingPath.FindChar(TEXT('/'), FirstSlashIndex) && FirstSlashIndex > 0) // check if class name is present
	{
		OutClassName = Path.Left(FirstSlashIndex).TrimEnd();
		WorkingPath = WorkingPath.RightChop(FirstSlashIndex);
	}
	
	FPaths::Split(WorkingPath, OutPath, OutName, OutExtension);
	auto Test = FPackageName::GetShortName(Path);

	return true;
	//return SplitObjectPath_ObjectPath(AssetData.ToSoftObjectPath(), OutPath, OutName, OutClassName, OutExtension);
}

UObject* UEditorPlusAssetLibrary::GetOrCreateAsset(const FString& Path, const FString& Name, TSubclassOf<UObject> Class)
{
	auto& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();
	const auto ObjectPath = FName(*(Path + TEXT("/") + Name));
	const auto Asset = AssetRegistry.GetAssetByObjectPath(ObjectPath);
	if(Asset.IsValid())
		return Asset.GetAsset();

	auto& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	return AssetTools.CreateAsset(Name, Path, Class, nullptr);
}

UStringAssetUserData* UEditorPlusAssetLibrary::GetOrAddStringUserData(UObject* Object)
{
    check(Object);

    if (auto AssetUserData = Cast<IInterface_AssetUserData>(Object))
    {
        auto Result = Cast<UStringAssetUserData>(AssetUserData->GetAssetUserDataOfClass(UStringAssetUserData::StaticClass()));
        if (!Result)
        {
            Result = NewObject<UStringAssetUserData>();
            AssetUserData->AddAssetUserData(Result);
        }

        return Result;
    }

    return nullptr;
}

TArray<UObject*> UEditorPlusAssetLibrary::GetSelected()
{
	auto& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	TArray<FAssetData> SelectedAssets;
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);

	TArray<UObject*> Result;
	for (auto& AssetData : SelectedAssets)
		Result.Add(AssetData.GetAsset());

	return Result;
}

void UEditorPlusAssetLibrary::RenameAsset(UObject* Asset, const FString& NewName)
{
	auto& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));

	TArray<FAssetRenameData> AssetsAndNames;
	const auto PackagePath = FPackageName::GetLongPackagePath(Asset->GetOutermost()->GetName());
	AssetsAndNames.Add(FAssetRenameData(Asset, PackagePath, NewName));
	
	AssetToolsModule.Get().RenameAssetsWithDialog(AssetsAndNames);
}

TArray<FString> UEditorPlusAssetLibrary::GetAllPackageFiles()
{
	TArray<FString> Result;
	
	TArray<FString> AllPackageFileNames;
	FEditorFileUtils::FindAllPackageFiles(AllPackageFileNames);
	for(auto PackageIdx = 0; PackageIdx < AllPackageFileNames.Num(); PackageIdx++)
	{
		const auto& FileName = AllPackageFileNames[PackageIdx];
		FString LongPackageName;
		if(FPackageName::TryConvertFilenameToLongPackageName(FileName, LongPackageName))
		{
			Result.Add(LongPackageName);
		}
	}

	return Result;
}

TArray<FString> UEditorPlusAssetLibrary::GetAllAssetFiles()
{
	TArray<FString> Result;
	
	auto AllPackageFileNames = GetAllPackageFiles();
	for(auto& FileName : AllPackageFileNames)
	{
		if(FPaths::GetExtension(FileName, true) == FPackageName::GetAssetPackageExtension())
			Result.Add(FileName);
	}

	return Result;
}

TArray<FString> UEditorPlusAssetLibrary::GetAllMapFiles()
{
		TArray<FString> Result;
	
	auto AllPackageFileNames = GetAllPackageFiles();
	for(auto& FileName : AllPackageFileNames)
	{
		if(FPaths::GetExtension(FileName, true) == FPackageName::GetMapPackageExtension())
			Result.Add(FileName);
	}

	return Result;
}

#pragma region Not Working

// NOTE: This doesn't seem to work
// Returns number of assets affected
TArray<FString> UEditorPlusAssetLibrary::GetWithCustomVersion(const FGuid VersionGuid, const int32 MinVersion, const int32 MaxVersion)
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

    auto TaskLabel = FText::FromString(TEXT("Checking Assets for Custom Version"));
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
            const auto Object = Asset.GetAsset();
			if (Object)
			{
                const auto ObjectsCustomVersion = Object->GetLinkerCustomVersion(VersionGuid);
                const auto bMatches = ObjectsCustomVersion != -1 && ObjectsCustomVersion >= MinVersion && ObjectsCustomVersion <= MaxVersion;
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

int32 UEditorPlusAssetLibrary::ResaveWithCustomVersion(FGuid VersionGuid, int32 MinVersion, int32 MaxVersion)
{
	//AssetToolsModule.Get().Vers

    const FString InputFilePath;
    const auto Package = LoadPackage(nullptr, *InputFilePath, LOAD_None);
	if (Package)
	{
        const FString OutputFilePath;
		UPackage::SavePackage(Package, nullptr, EObjectFlags::RF_Standalone, *OutputFilePath);
	}

	return -1;
}

#pragma endregion Not Working
