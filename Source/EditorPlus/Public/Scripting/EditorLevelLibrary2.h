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
    static FString GetCurrentLevelPath();

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
	static void AddStreamingLevel(const FName& Path);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility")
	static TArray<AActor*> GetActorsInSublevel(const FName& SublevelPath);
};
