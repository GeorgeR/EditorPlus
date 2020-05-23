#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameFramework/Actor.h"

#include "EditorPlusLevelLibrary.generated.h"

class FLevelModel;
class FLevelCollectionModel;
typedef TArray<TSharedPtr<FLevelModel>> FLevelModelList;

UCLASS()
class EDITORPLUS_API UEditorPlusLevelLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
	/** Loads the level if it exists, creates it if it doesn't, optionally based on a template. */
	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level|Build")
	static void LoadOrCreateLevel(const FString& AssetPath, const FString& TemplateAssetPath = TEXT(""));

public:
	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level|Build")
	static void BuildHLOD(bool bForceRebuild = false);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level|Build")
	static void BuildHLODClusters(bool bRebuild = false);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level|Build", meta = (DisplayName = "Is HLOD Built"))
	static bool IsHLODBuilt();

public:
	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
	static bool DeselectAllActorsOfClass(TSubclassOf<AActor> ActorClass);
	
    UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
    static bool GetOrCreateFolder(FName Path);

    UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
    static bool MoveActorsToFolder(TArray<AActor*> Actors, FName Path);

    UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
    static FString CurrentLevelName();
    
    UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
    static FString GetCurrentLevelPath();

    UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
    static void SetWorldOrigin(const int32& X, const int32& Y, const int32& Z);
    
    UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
    static void AddStreamingLevel(const FName& Path);

    UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
    static TArray<AActor*> GetActorsInSublevel(const FName& SublevelPath);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
	static TArray<FString> GetSubLevels();

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
	static TArray<ULevel*> LoadSubLevels(const TArray<FString>& Paths);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
	static ULevel* LoadSubLevel(const FString& Path);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
	static void LoadSubLevelAsync(const FString& Path);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
	static void UnloadSubLevels(const TArray<ULevel*>& Levels, bool bSaveIfDirty = true);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
	static TArray<FIntPoint> GetSubLevelPositions(const TArray<ULevel*>& Levels);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
	static FIntPoint GetSubLevelPosition(ULevel* Level);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
	static void SetSubLevelPositions(const TArray<ULevel*>& Levels, const FIntPoint& Position);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
	static void SetSubLevelPosition(ULevel* Levels, const FIntPoint& Position);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Level")
	static void Test();

public:
    /** World Browser Functions */

    UFUNCTION(BlueprintCallable, Category = "EditorPlus|WorldBrowser")
    static FIntPoint GetWorldSize();

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|WorldBrowser")
	static void LoadLevel(const FString& Level);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|WorldBrowser")
	static void LoadLevels(const TArray<FString>& Levels);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|WorldBrowser")
	static void UnloadLevels(const TArray<FString>& Levels);

    UFUNCTION(BlueprintCallable, Category = "EditorPlus|WorldBrowser")
    static const TArray<FString> GetAllLevels();

    UFUNCTION(BlueprintCallable, Category = "EditorPlus|WorldBrowser")
    static const TArray<FString> GetSelectedLevels();

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|WorldBrowser")
	static FIntVector GetLevelLocation(const FString& Level);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|WorldBrowser")
	static void SetLevelLocation(const FString& Level, const FIntVector& Location);

#pragma region Experimental
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EditorPlus|TaskGraph")
	static bool IsThreadProcessingTasks();

#pragma endregion Experimental

private:
	static void DoForWorldModel(TFunction<void(TSharedPtr<FLevelCollectionModel>)> Func);

    template <typename T>
    static T DoForWorldModel(TFunction<T(TSharedPtr<FLevelCollectionModel>)> Func);

    static FLevelModelList ToLevelModelList(const TArray<FString>& List);
    static TArray<FString> FromLevelModelList(const FLevelModelList& List);
};
