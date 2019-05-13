#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EditorLevelLibrary2.generated.h"

class FLevelModel;
class FLevelCollectionModel;
typedef TArray<TSharedPtr<FLevelModel>> FLevelModelList;

UCLASS()
class EDITORPLUS_API UEditorLevelLibrary2
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
    static bool GetOrCreateFolder(FName Path);

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
    static bool MoveActorsToFolder(TArray<AActor*> Actors, FName Path);

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
    static FString CurrentLevelName();
    
    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
    static FString GetCurrentLevelPath();

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
    static void SetWorldOrigin(const int32& X, const int32& Y, const int32& Z);
    
    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
    static void AddStreamingLevel(const FName& Path);

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
    static TArray<AActor*> GetActorsInSublevel(const FName& SublevelPath);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
	static TArray<FString> GetSubLevels();

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
	static TArray<ULevel*> LoadSubLevels(const TArray<FString>& Paths);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
	static ULevel* LoadSubLevel(const FString& Path);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
	static void UnloadSubLevels(const TArray<ULevel*>& Levels, bool bSaveIfDirty = true);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
	static TArray<FIntPoint> GetSubLevelPositions(const TArray<ULevel*>& Levels);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
	static FIntPoint GetSubLevelPosition(ULevel* Level);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
	static void SetSubLevelPositions(const TArray<ULevel*>& Levels, const FIntPoint& Position);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
	static void SetSubLevelPosition(ULevel* Levels, const FIntPoint& Position);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
	static void Test();

public:
    /** World Browser Functions */

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | World Browser")
    static FIntPoint GetWorldSize();

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | World Browser")
	static void LoadLevel(const FString& Level);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | World Browser")
	static void LoadLevels(const TArray<FString>& Levels);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | World Browser")
	static void UnloadLevels(const TArray<FString>& Levels);

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | World Browser")
    static const TArray<FString> GetAllLevels();

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | World Browser")
    static const TArray<FString> GetSelectedLevels();

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | World Browser")
	static FIntVector GetLevelLocation(const FString& Level);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | World Browser")
	static void SetLevelLocation(const FString& Level, const FIntVector& Location);

private:
	static void DoForWorldModel(TFunction<void(TSharedPtr<FLevelCollectionModel>)> Func);

    template <typename T>
    static T DoForWorldModel(TFunction<T(TSharedPtr<FLevelCollectionModel>)> Func);

    static FLevelModelList ToLevelModelList(const TArray<FString>& List);
    static TArray<FString> FromLevelModelList(const FLevelModelList& List);
};
