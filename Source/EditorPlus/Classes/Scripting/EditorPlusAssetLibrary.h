#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/AssetRegistryInterface.h"
#include "AssetData.h"

#include "EditorPlusAssetLibrary.generated.h"

class IAssetRegistry;
class IAssetTools;
class UFactory;
class UStringAssetUserData;
class UJsonAssetUserData;

UCLASS()
class EDITORPLUS_API UEditorPlusAssetLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset")
	static bool GetAssetLike(const FString& Path, UPARAM(DisplayName = "Asset") FAssetData& OutAsset, UPARAM(DisplayName = "Path") FString& OutPath, TSubclassOf<UObject> Class = nullptr, const FString& Prefix = TEXT(""), const FString& Name = TEXT(""), const FString& Suffix = TEXT(""));

public:
	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset", meta = (DisplayName = "Reimport Asset (Object)"))
    static bool ReimportAsset_Object(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset", meta = (DisplayName = "Reimport Asset (AssetData)"))
    static bool ReimportAsset_AssetData(const FAssetData& AssetData);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset", meta = (DisplayName = "Reimport Asset (String)"))
    static bool ReimportAsset_String(const FString& Path);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset", meta = (DisplayName = "Reimport Asset From File (Object)"))
    static bool ReimportAssetFromFile_Object(UObject* Object, const FString& FilePath);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset", meta = (DisplayName = "Reimport Asset From File (AssetData)"))
    static bool ReimportAssetFromFile_AssetData(const FAssetData& AssetData, const FString& FilePath);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset", meta = (DisplayName = "Reimport Asset From File (String)"))
    static bool ReimportAssetFromFile_String(const FString& Path, const FString& FilePath);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EditorPlus|Asset", meta = (DisplayName = "Split Asset Path (Object)"))
    static bool SplitObjectPath_Object(UObject* Object, UPARAM(DisplayName = "Path Part") FString& OutPath, UPARAM(DisplayName = "Name Part") FString& OutName, UPARAM(DisplayName = "Class Name") FString& OutClassName, UPARAM(DisplayName = "Extension") FString& OutExtension);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EditorPlus|Asset", meta = (DisplayName = "Split Asset Path (Name)"))
    static bool SplitObjectPath_Name(const FName& Path, UPARAM(DisplayName = "Path Part") FString& OutPath, UPARAM(DisplayName = "Name Part") FString& OutName, UPARAM(DisplayName = "Class Name") FString& OutClassName, UPARAM(DisplayName = "Extension") FString& OutExtension);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EditorPlus|Asset", meta = (DisplayName = "Split Asset Path (AssetData)"))
    static bool SplitObjectPath_AssetData(const FAssetData& AssetData, UPARAM(DisplayName = "Path Part") FString& OutPath, UPARAM(DisplayName = "Name Part") FString& OutName, UPARAM(DisplayName = "Class Name") FString& OutClassName, UPARAM(DisplayName = "Extension") FString& OutExtension);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EditorPlus|Asset", meta = (DisplayName = "Split Asset Path (SoftObjectPath)"))
    static bool SplitObjectPath_ObjectPath(const FSoftObjectPath& ObjectPath, UPARAM(DisplayName = "Path Part") FString& OutPath, UPARAM(DisplayName = "Name Part") FString& OutName, UPARAM(DisplayName = "Class Name") FString& OutClassName, UPARAM(DisplayName = "Extension") FString& OutExtension);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EditorPlus|Asset", meta = (DisplayName = "Split Asset Path (String)"))
    static bool SplitObjectPath_String(const FString& Path, UPARAM(DisplayName = "Path Part") FString& OutPath, UPARAM(DisplayName = "Name Part") FString& OutName, UPARAM(DisplayName = "Class Name") FString& OutClassName, UPARAM(DisplayName = "Extension") FString& OutExtension);

public:
	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset")
	static UObject* GetOrCreateAsset(const FString& Path, const FString& Name, TSubclassOf<UObject> Class);
	
public:
    UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset")
    static UStringAssetUserData* GetOrAddStringUserData(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset")
    static UJsonAssetUserData* GetOrAddJsonUserData(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset")
	static TArray<UObject*> GetSelected();

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset", meta = (DisplayName = "Rename"))
	static void RenameAsset(UObject* Asset, const FString& NewName);

public:
	/** Looks for package files in the known content paths on disk. */
	/** @note Wrapper for FEditorFileUtils::FindAllPackageFiles() */
	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset")
	static TArray<FString> GetAllPackageFiles();

	/** Looks for asset files (not maps) in the known content paths on disk. */
	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset")
	static TArray<FString> GetAllAssetFiles();

	/** Looks for map files (not assets) in the known content paths on disk. */
	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset")
	static TArray<FString> GetAllMapFiles();

#pragma region Not Working
private:
	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset")
	static TArray<FString> GetWithCustomVersion(FGuid VersionGuid, int32 MinVersion, int32 MaxVersion);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset")
	static int32 ResaveWithCustomVersion(FGuid VersionGuid, int32 MinVersion, int32 MaxVersion);
#pragma endregion Not Working

#pragma region Unimplemented (stubbed)
private:
	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Asset")
	static FString GetCallingPath(UObject* Context) { return TEXT(""); }
#pragma endregion Unimplemented (stubbed)

private:
	static IAssetRegistry& GetAssetRegistry();
	static IAssetTools& GetAssetTools();
};