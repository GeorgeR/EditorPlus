#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssetData.h"

#include "EditorAssetLibrary2.generated.h"

class UStringAssetUserData;

UCLASS()
class EDITORPLUS_API UEditorAssetLibrary2
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Asset")
	static TArray<FString> GetWithCustomVersion(FGuid VersionGuid, int32 MinVersion, int32 MaxVersion);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Asset")
	static int32 ResaveWithCustomVersion(FGuid VersionGuid, int32 MinVersion, int32 MaxVersion);

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Asset")
    static UStringAssetUserData* GetOrAddStringUserData(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Asset")
	static TArray<UObject*> GetSelected();

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Asset", meta = (DisplayName = "Rename"))
	static void RenameAsset(UObject* Asset, const FString& NewName);
};
