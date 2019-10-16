#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssetRegistryInterface.h"
#include "AssetData.h"

#include "EditorPlusAssetLibrary.generated.h"

class UFactory;
class UStringAssetUserData;

UCLASS()
class EDITORPLUS_API UEditorPlusAssetLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

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
};
