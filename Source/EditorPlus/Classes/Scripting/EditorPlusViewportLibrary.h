#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EditorPlusViewportLibrary.generated.h"

UCLASS()
class EDITORPLUS_API UEditorPlusViewportLibrary 
    : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EditorPlus|Viewport")
    static int32 GetActiveViewportIndex();

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Viewport")
	static void SetViewportLocationAndRotation(const int32 ViewportIndex, FVector Location, FRotator Rotation);
};