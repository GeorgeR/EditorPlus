#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StringAssetUserData.h"

#include "EditorPlusFileLibrary.generated.h"

UCLASS()
class EDITORPLUS_API UEditorPlusFileLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "EditorPlus|File")
    static bool ReadTextFile(const FString& Path, UPARAM(DisplayName = "Result") FString& OutResult);
	
    //UFUNCTION(BlueprintCallable, Category = "Editor Plus|File")
    //static UStringAssetUserData* GetOrAddStringUserData(UObject* Object);
};
