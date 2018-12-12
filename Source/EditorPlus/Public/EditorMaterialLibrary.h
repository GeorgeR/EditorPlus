#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssetData.h"

#include "EditorMaterialLibrary.generated.h"

class UMaterialInterface;
class UTexture;

UCLASS()
class EDITORPLUS_API UEditorMaterialLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Material", meta = (DisplayName = "CreateMaterialInstance"))
    static UMaterialInterface* CreateMaterialInstance_FromPath(const FString& BaseMaterialPath, const FString& DestinationPath);

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Material")
    static UMaterialInterface* CreateMaterialInstance(UMaterialInterface* BaseMaterial, const FString& DestinationPath);

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Material", meta = (DisplayName = "SetParameter"))
    static bool SetVectorParameter(UMaterialInterface* Material, const FName Name, FLinearColor Value);

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Material", meta = (DisplayName = "SetParameter"))
    static bool SetVectorParameter_FromVector(UMaterialInterface* Material, const FName Name, FVector Value);

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Material", meta = (DisplayName = "SetParameter"))
    static bool SetScalarParameter(UMaterialInterface* Material, const FName Name, float Value);

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Material", meta = (DisplayName = "SetParameter"))
    static bool SetTextureParameter(UMaterialInterface* Material, const FName Name, UTexture* Value);
};