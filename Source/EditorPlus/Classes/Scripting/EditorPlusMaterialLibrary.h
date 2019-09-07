#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssetData.h"

#include "EditorPlusMaterialLibrary.generated.h"

class UMaterialInterface;
class UTexture;

UCLASS()
class EDITORPLUS_API UEditorPlusMaterialLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Editor Plus | Material", meta = (DisplayName = "Create Material Instance (from asset path)"))
    static UMaterialInterface* CreateMaterialInstance_FromPath(const FString& BaseMaterialPath, const FString& DestinationPath);

    UFUNCTION(BlueprintCallable, Category = "Editor Plus | Material")
    static UMaterialInterface* CreateMaterialInstance(UMaterialInterface* BaseMaterial, const FString& DestinationPath);

    UFUNCTION(BlueprintCallable, Category = "Editor Plus | Material", meta = (DisplayName = "Set Parameter (color)"))
    static bool SetVectorParameter(UMaterialInterface* Material, const FName Name, FLinearColor Value);

    UFUNCTION(BlueprintCallable, Category = "Editor Plus | Material", meta = (DisplayName = "Set Parameter (vector)"))
    static bool SetVectorParameter_FromVector(UMaterialInterface* Material, const FName Name, FVector Value);

    UFUNCTION(BlueprintCallable, Category = "Editor Plus | Material", meta = (DisplayName = "Set Parameter (scalar)"))
    static bool SetScalarParameter(UMaterialInterface* Material, const FName Name, float Value);

    UFUNCTION(BlueprintCallable, Category = "Editor Plus | Material", meta = (DisplayName = "Set Parameter (texture)"))
    static bool SetTextureParameter(UMaterialInterface* Material, const FName Name, UTexture* Value);
};