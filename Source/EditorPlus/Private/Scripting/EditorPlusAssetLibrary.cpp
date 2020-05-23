#include "Scripting/EditorPlusAssetLibrary.h"

#include "Interfaces/Interface_AssetUserData.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Modules/ModuleManager.h"
#include "AssetData.h"
#include "AssetToolsModule.h"
#include "AssetRegistryModule.h"
#include "FileHelpers.h"
#include "IAssetTools.h"
#include "PackageTools.h"
#include "EditorReimportHandler.h"
#include "Engine/StaticMesh.h"
#include "Factories/ReimportFbxStaticMeshFactory.h"

#include "StringAssetUserData.h"
#include "JsonAssetUserData.h"

// fuzzy search
bool UEditorPlusAssetLibrary::GetAssetLike(const FString& Path, FAssetData& OutAsset, FString& OutPath, TSubclassOf<UObject> Class, const FString& Prefix, const FString& Name, const FString& Suffix)
{
	check(!Path.IsEmpty());
	check(Class != nullptr || !Name.IsEmpty());

	TArray<FAssetData> Assets;
	auto& AssetRegistry = GetAssetRegistry();
	if(!AssetRegistry.GetAssetsByPath(*Path, Assets, true))
		return false;

	FARFilter Filter;
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;
	if(Class != nullptr)
		Filter.ClassNames.Add(*Class->GetName());

	AssetRegistry.RunAssetsThroughFilter(Assets, Filter);

	if(Assets.Num() == 0)
		return false;

	for (auto AssetDataIdx = Assets.Num() - 1; AssetDataIdx >= 0; --AssetDataIdx)
	{
		const auto& AssetData = Assets[AssetDataIdx];
		auto AssetName = AssetData.AssetName.ToString();

		if(!Prefix.IsEmpty())
			if(AssetName.StartsWith(Prefix))
				AssetName.RemoveFromStart(Prefix);
			else
				continue;

		if(!Suffix.IsEmpty() && !AssetName.EndsWith(Suffix))
			if(AssetName.EndsWith(Prefix))
				AssetName.RemoveFromEnd(Prefix);
			else
				continue;

		if(!Name.IsEmpty())
			if(AssetName.MatchesWildcard(Name))
			{
				OutAsset = AssetData;
				OutPath = AssetData.GetFullName();

				FString PackageRoot;
				FString ClassName;
				FString PackageName;
				FString ObjectName;
				FString SubObjectName;
				FPackageName::SplitFullObjectPath(OutPath, ClassName, PackageName, ObjectName, SubObjectName);
				OutPath = PackageName;
				return true;
			}
	}

	OutPath = FPaths::Combine(Path, Prefix + Name + Suffix);
	
	return false;
}

bool UEditorPlusAssetLibrary::ReimportAsset_Object(UObject* Object)
{
	check(Object);

	if(!Object->IsAsset())
		return false;

	TArray<UObject*> Objects;
	Objects.Add(Object);
	FReimportManager::Instance()->ValidateAllSourceFileAndReimport(Objects, true, INDEX_NONE, false);
	return true;
}

bool UEditorPlusAssetLibrary::ReimportAsset_AssetData(const FAssetData& AssetData)
{
	check(AssetData.IsValid());

	auto* Object = AssetData.GetAsset();
	return ReimportAsset_Object(Object);
}

bool UEditorPlusAssetLibrary::ReimportAsset_String(const FString& Path)
{
	check(!Path.IsEmpty());

	auto* Object = StaticLoadObject(UObject::StaticClass(), nullptr, *Path, nullptr, LOAD_None, nullptr, true, nullptr);
	if(Object == nullptr)
	{
		FSoftObjectPath ObjectPath(Path);
		if(ObjectPath.FixupCoreRedirects())
			Object = LoadObject<UObject>(nullptr, *ObjectPath.ToString());
	}

	return ReimportAsset_Object(Object);
}

bool UEditorPlusAssetLibrary::ReimportAssetFromFile_Object(UObject* Object, const FString& FilePath)
{
	check(Object);

	if(!Object->IsAsset())
		return false;

	TArray<UObject*> Objects;
	Objects.Add(Object);

	FReimportHandler* Handler = nullptr;
	if(Object->GetClass() == UStaticMesh::StaticClass())
		Handler = StaticCast<FReimportHandler*>(GetMutableDefault<UReimportFbxStaticMeshFactory>());
	
	return FReimportManager::Instance()->Reimport(Object, false, false, FilePath, Handler);
}

bool UEditorPlusAssetLibrary::ReimportAssetFromFile_AssetData(const FAssetData& AssetData, const FString& FilePath)
{
	check(AssetData.IsValid());

	auto* Object = AssetData.GetAsset();
	return ReimportAssetFromFile_Object(Object, FilePath);
}

bool UEditorPlusAssetLibrary::ReimportAssetFromFile_String(const FString& Path, const FString& FilePath)
{
	check(!Path.IsEmpty());

	auto* Object = StaticLoadObject(UObject::StaticClass(), nullptr, *Path, nullptr, LOAD_None, nullptr, true, nullptr);
	if(Object == nullptr)
	{
		FSoftObjectPath ObjectPath(Path);
		if(ObjectPath.FixupCoreRedirects())
			Object = LoadObject<UObject>(nullptr, *ObjectPath.ToString());
	}

	return ReimportAssetFromFile_Object(Object, FilePath);
}

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
	auto& AssetRegistry = GetAssetRegistry();
	const auto ObjectPath = FName(*(Path + TEXT("/") + Name));
	const auto Asset = AssetRegistry.GetAssetByObjectPath(ObjectPath);
	if(Asset.IsValid())
		return Asset.GetAsset();

	auto& AssetTools = GetAssetTools();
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

UJsonAssetUserData* UEditorPlusAssetLibrary::GetOrAddJsonUserData(UObject* Object)
{
	check(Object);

    if (auto AssetUserData = Cast<IInterface_AssetUserData>(Object))
    {
        auto Result = Cast<UJsonAssetUserData>(AssetUserData->GetAssetUserDataOfClass(UJsonAssetUserData::StaticClass()));
        if (!Result)
        {
            Result = NewObject<UJsonAssetUserData>();
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
	TArray<FAssetRenameData> AssetsAndNames;
	const auto PackagePath = FPackageName::GetLongPackagePath(Asset->GetOutermost()->GetName());
	AssetsAndNames.Add(FAssetRenameData(Asset, PackagePath, NewName));
	
	GetAssetTools().RenameAssetsWithDialog(AssetsAndNames);
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

	auto& AssetRegistry = GetAssetRegistry();
	if (AssetRegistry.IsLoadingAssets())
		return AffectedAssets;

	FARFilter Filter;
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;

	TArray<FAssetData> Assets;
	AssetRegistry.GetAllAssets(Assets);

	if (Assets.Num() == 0)
		return AffectedAssets;

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

IAssetRegistry& UEditorPlusAssetLibrary::GetAssetRegistry()
{
	auto& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	return AssetRegistryModule.Get();
}

IAssetTools& UEditorPlusAssetLibrary::GetAssetTools()
{
	auto& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
	return AssetToolsModule.Get();
}
