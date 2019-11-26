#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EditorPlusLODLibrary.generated.h"

UCLASS()
class EDITORPLUS_API UEditorPlusLODLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | HLOD")
    static bool ShouldGenerateCluster(UPARAM(DisplayName = "Actor") AActor* InActor, const int32 HLODLevelIndex = 0);
};
