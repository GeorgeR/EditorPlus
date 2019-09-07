#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/ActorComponent.h"

#include "EditorPlusBlueprintLibrary.generated.h"

class UBlueprintGeneratedClass;

UCLASS()
class EDITORPLUS_API UEditorPlusBlueprintLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** From user PhilippOkoampah: https://answers.unrealengine.com/questions/558236/how-to-get-a-component-from-a-classdefaultobject.html */
    UFUNCTION(BlueprintCallable, Category = "Editor Plus | Blueprint")
    static UActorComponent* FindDefaultComponentByClass(UObject* Blueprint, const TSubclassOf<UActorComponent> ComponentClass);

    /** https://answers.unrealengine.com/questions/558236/how-to-get-a-component-from-a-classdefaultobject.html */
    UFUNCTION(BlueprintCallable, Category = "Editor Plus | Blueprint")
    static TArray<UActorComponent*> FindDefaultComponentsByClass(UObject* Blueprint, const TSubclassOf<UActorComponent> ComponentClass);
};
