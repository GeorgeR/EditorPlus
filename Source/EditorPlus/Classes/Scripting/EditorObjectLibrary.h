#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EditorObjectLibrary.generated.h"

// FROM LevelCollectionModel.h
//
// Helper struct to temporally make specified UObject immune to dirtying
//
//struct FUnmodifiableObject
//{
//	FUnmodifiableObject(UObject* InObject)
//		: ImmuneObject(InObject)
//		, bTransient(InObject->HasAnyFlags(RF_Transient))
//	{
//		if (!bTransient)
//		{
//			ImmuneObject->SetFlags(RF_Transient);
//		}
//	}
//
//	~FUnmodifiableObject()
//	{
//		if (!bTransient)
//		{
//			ImmuneObject->ClearFlags(RF_Transient);
//		}
//	}
//
//private:
//	UObject*		ImmuneObject;
//	bool			bTransient;
//};

UCLASS()
class EDITORPLUS_API UEditorObjectLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Object")
    static void AddToRoot(UObject* Object);
    
    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Object")
    static void RemoveFromRoot(UObject* Object);
};
