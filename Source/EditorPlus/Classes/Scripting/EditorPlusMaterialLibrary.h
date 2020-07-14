#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssetData.h"

#include "EditorPlusMaterialLibrary.generated.h"

class UMaterialInterface;
class UMaterialInstanceConstant;
class UTexture;

UCLASS()
class EDITORPLUS_API UEditorPlusMaterialLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "EditorPlus|Material", meta = (DisplayName = "Create Material Instance (from asset path)"))
    static UMaterialInterface* CreateMaterialInstance_FromPath(const FString& BaseMaterialPath, const FString& DestinationPath);

    UFUNCTION(BlueprintCallable, Category = "EditorPlus|Material")
    static UMaterialInterface* CreateMaterialInstance(UMaterialInterface* BaseMaterial, const FString& DestinationPath);

    UFUNCTION(BlueprintCallable, Category = "EditorPlus|Material", meta = (DisplayName = "Set Parameter (Color)"))
    static bool SetVectorParameter(UMaterialInterface* Material, const FName Name, FLinearColor Value);

    UFUNCTION(BlueprintCallable, Category = "EditorPlus|Material", meta = (DisplayName = "Set Parameter (Vector)"))
    static bool SetVectorParameter_FromVector(UMaterialInterface* Material, const FName Name, FVector Value);

    UFUNCTION(BlueprintCallable, Category = "EditorPlus|Material", meta = (DisplayName = "Set Parameter (Scalar)"))
    static bool SetScalarParameter(UMaterialInterface* Material, const FName Name, float Value);

    UFUNCTION(BlueprintCallable, Category = "EditorPlus|Material", meta = (DisplayName = "Set Parameter (Texture)"))
    static bool SetTextureParameter(UMaterialInterface* Material, const FName Name, UTexture* Value);

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Material")
    static FLinearColor GetAverageBaseColor(UMaterialInterface* Material);

private:
    template <typename T>
    static bool SetMaterialInstanceParameter(UMaterialInterface* Material, const FName Name, T Value, TFunctionRef<bool(UMaterialInstanceConstant*, FName, T)> Func);
};