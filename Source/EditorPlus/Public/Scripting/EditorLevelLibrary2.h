#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EditorLevelLibrary2.generated.h"

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

private:

};
