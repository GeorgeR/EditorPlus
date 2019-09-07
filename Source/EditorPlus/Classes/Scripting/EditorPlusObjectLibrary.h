#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EditorPlusObjectLibrary.generated.h"

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
class EDITORPLUS_API UEditorPlusObjectLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Editor Plus | Object")
    static void AddToRoot(UObject* Object);
    
    UFUNCTION(BlueprintCallable, Category = "Editor Plus | Object")
    static void RemoveFromRoot(UObject* Object);

    UFUNCTION(BlueprintCallable, Category = "Editor Plus | Object", meta = (DisplayName = "Set Property (bool)"))
    static bool SetProperty_Bool(UObject* Object, const FName Name, bool Value) { return SetProperty(Object, Name, Value); }

private:
    static void* GetPropertyValuePtr(UProperty* Property, UProperty* Outer, void* Data, int32 ArrayIndex);

    template <typename ValueType>
    static bool SetProperty(UObject* Object, const FName Name, const ValueType& Value);

    template <typename PropertyType, typename ValueType>
    static bool SetPropertyValue(UObject* Object, PropertyType* Property, int32 ArrayIndex, const ValueType& Value);
};
