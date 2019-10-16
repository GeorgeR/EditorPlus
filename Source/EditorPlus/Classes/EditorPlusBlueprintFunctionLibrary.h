#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EditorPlusBlueprintFunctionLibrary.generated.h"

/** Inherit this for Editor-Only libraries */
UCLASS(BlueprintType, Blueprintable, Abstract)
class EDITORPLUS_API UEditorPlusBlueprintFunctionLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UEditorPlusBlueprintFunctionLibrary();

	bool IsEditorOnly() const override { return true; }
};
