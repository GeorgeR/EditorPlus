#include "Scripting/EditorBlueprintLibrary2.h"

#include "GameFramework/Actor.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/SCS_Node.h"

UActorComponent* UEditorBlueprintLibrary2::FindDefaultComponentByClass(TSubclassOf<AActor> ActorClass, const TSubclassOf<UActorComponent> ComponentClass)
{
    if (!IsValid(ActorClass))
        return nullptr;

    if (!IsValid(ComponentClass))
        return nullptr;

    auto ActorCDO = ActorClass->GetDefaultObject<AActor>();
    auto FoundComponent = ActorCDO->FindComponentByClass(ComponentClass);

    if (FoundComponent != nullptr)
        return FoundComponent;

    auto RootBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(ActorClass);
    do 
    {
        auto ActorBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(ActorClass);
        if (!ActorBlueprintGeneratedClass)
            return nullptr;

        const TArray<USCS_Node*>& ActorBlueprintNodes = ActorBlueprintGeneratedClass->SimpleConstructionScript->GetAllNodes();
        for (auto& Node : ActorBlueprintNodes)
            if (Node->ComponentClass->IsChildOf(ComponentClass))
                return Node->GetActualComponentTemplate(RootBlueprintGeneratedClass);

        ActorClass = Cast<UClass>(ActorClass->GetSuperStruct());
    } while (ActorClass != AActor::StaticClass());

    return nullptr;
}

TArray<UActorComponent*> UEditorBlueprintLibrary2::FindDefaultComponentsByClass(TSubclassOf<AActor> ActorClass, const TSubclassOf<UActorComponent> ComponentClass)
{
    TArray<UActorComponent*> Result;

    if (!IsValid(ActorClass))
        return Result;

    if (!IsValid(ComponentClass))
        return Result;

    auto ActorCDO = ActorClass->GetDefaultObject<AActor>();
    Result = ActorCDO->GetComponentsByClass(ComponentClass);

    auto RootBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(ActorClass);
    do
    {
        auto ActorBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(ActorClass);
        if (!ActorBlueprintGeneratedClass)
            return Result;

        const TArray<USCS_Node*>& ActorBlueprintNodes = ActorBlueprintGeneratedClass->SimpleConstructionScript->GetAllNodes();
        for (auto& Node : ActorBlueprintNodes)
            if (Node->ComponentClass->IsChildOf(ComponentClass))
                Result.Add(Node->GetActualComponentTemplate(RootBlueprintGeneratedClass));

        ActorClass = Cast<UClass>(ActorClass->GetSuperStruct());
    } while (ActorClass != AActor::StaticClass());

    return Result;
}
