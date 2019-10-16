#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EditorPlusStringLibrary.generated.h"

UCLASS()
class EDITORPLUS_API UEditorPlusStringLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "EditorPlus|String")
    static FString StripPrefix(const FString& String, FString Prefix, FString Separator = TEXT("_"));

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|String")
    static FString StripSuffix(const FString& String, FString Suffix, FString Separator = TEXT("_"));

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|String")
    static FString AddPrefix(const FString& String, FString Prefix, FString Separator = TEXT("_"));

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|String")
    static FString AddSuffix(const FString& String, FString Suffix, FString Separator = TEXT("_"));
};
