#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EditorPlusAssetLibrary.generated.h"

class UStringAssetUserData;

UCLASS()
class EDITORPLUS_API UEditorPlusAssetLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Editor Plus | Asset")
    static UStringAssetUserData* GetOrAddStringUserData(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Editor Plus | Asset")
	static TArray<UObject*> GetSelected();

	UFUNCTION(BlueprintCallable, Category = "Editor Plus | Asset", meta = (DisplayName = "Rename"))
	static void RenameAsset(UObject* Asset, const FString& NewName);

#pragma region Not Working
private:
	UFUNCTION(BlueprintCallable, Category = "Editor Plus | Asset")
	static TArray<FString> GetWithCustomVersion(FGuid VersionGuid, int32 MinVersion, int32 MaxVersion);

	UFUNCTION(BlueprintCallable, Category = "Editor Plus | Asset")
	static int32 ResaveWithCustomVersion(FGuid VersionGuid, int32 MinVersion, int32 MaxVersion);
#pragma endregion Not Working
};
