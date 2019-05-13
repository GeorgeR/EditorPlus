#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/ActorComponent.h"

#include "EditorBlueprintLibrary2.generated.h"

UCLASS()
class EDITORPLUS_API UEditorBlueprintLibrary2
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** From user PhilippOkoampah: https://answers.unrealengine.com/questions/558236/how-to-get-a-component-from-a-classdefaultobject.html */
    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Blueprint")
    static UActorComponent* FindDefaultComponentByClass(TSubclassOf<AActor> ActorClass, const TSubclassOf<UActorComponent> ComponentClass);

    /** https://answers.unrealengine.com/questions/558236/how-to-get-a-component-from-a-classdefaultobject.html */
    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Blueprint")
    static TArray<UActorComponent*> FindDefaultComponentsByClass(TSubclassOf<AActor> ActorClass, const TSubclassOf<UActorComponent> ComponentClass);
};
