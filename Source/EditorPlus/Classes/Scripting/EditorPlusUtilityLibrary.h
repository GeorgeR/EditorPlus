#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EditorPlusUtilityLibrary.generated.h"

UCLASS()
class EDITORPLUS_API UEditorPlusUtilityLibrary 
    : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "EditorPlus|Utility")
    static void CollectGarbage();

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Utility")
	static void Sleep(float Seconds = 1.0f);

#pragma region unimplemented
private:
	

#pragma endregion unimplemented
};
