#include "Scripting/EditorBlueprintLibrary2.h"

#include "GameFramework/Actor.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/SCS_Node.h"

UActorComponent* UEditorBlueprintLibrary2::FindDefaultComponentByClass(UObject* Blueprint, const TSubclassOf<UActorComponent> ComponentClass)
{
    if (!IsValid(Blueprint))
        return nullptr;

    if (!IsValid(ComponentClass))
        return nullptr;

    const auto BlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(Cast<UBlueprint>(Blueprint)->GeneratedClass.Get());
    check(BlueprintGeneratedClass);

    const auto& ActorBlueprintNodes = BlueprintGeneratedClass->SimpleConstructionScript->GetAllNodes();
    for (auto& Node : ActorBlueprintNodes)
        if (Node->ComponentClass->IsChildOf(ComponentClass))
            return Node->GetActualComponentTemplate(BlueprintGeneratedClass);

    return nullptr;
}

TArray<UActorComponent*> UEditorBlueprintLibrary2::FindDefaultComponentsByClass(UObject* Blueprint, const TSubclassOf<UActorComponent> ComponentClass)
{
    TArray<UActorComponent*> Result;

    if (!IsValid(Blueprint))
        return Result;

    if (!IsValid(ComponentClass))
        return Result;

    const auto BlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(Cast<UBlueprint>(Blueprint)->GeneratedClass.Get());
    check(BlueprintGeneratedClass);

    const auto& ActorBlueprintNodes = BlueprintGeneratedClass->SimpleConstructionScript->GetAllNodes();
    for (auto& Node : ActorBlueprintNodes)
        if (Node->ComponentClass->IsChildOf(ComponentClass))
            Result.Add(Node->GetActualComponentTemplate(BlueprintGeneratedClass));

    return Result;
}
