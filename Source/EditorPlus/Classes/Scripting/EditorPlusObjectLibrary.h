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
    UFUNCTION(BlueprintCallable, Category = "EditorPlus|Object")
    static void AddToRoot(UObject* Object);

    UFUNCTION(BlueprintCallable, Category = "EditorPlus|Object")
    static void RemoveFromRoot(UObject* Object);

public:
	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Object", meta = (DisplayName = "Set Property (byte)"))
	static bool SetProperty_Byte(UObject* Object, const FName Name, uint8 Value) { return SetProperty<UByteProperty, uint8>(Object, Name, Value); }

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Object", meta = (DisplayName = "Set Property (int)"))
    static bool SetProperty_Int32(UObject* Object, const FName Name, int32 Value) { return SetProperty<UIntProperty, int32>(Object, Name, Value); }

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Object", meta = (DisplayName = "Set Property (int64)"))
    static bool SetProperty_Int64(UObject* Object, const FName Name, int64 Value) { return SetProperty<UInt64Property, int64>(Object, Name, Value); }
	
	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Object", meta = (DisplayName = "Set Property (float)"))
	static bool SetProperty_Float(UObject* Object, const FName Name, float Value) { return SetProperty<UFloatProperty, float>(Object, Name, Value); }
	
	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Object", meta = (DisplayName = "Set Property (bool)"))
	static bool SetProperty_Bool(UObject* Object, const FName Name, bool Value) { return SetProperty<UBoolProperty, bool>(Object, Name, Value); }

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Object", meta = (DisplayName = "Set Property (Name)"))
    static bool SetProperty_Name(UObject* Object, const FName Name, FName Value) { return SetProperty<UNameProperty, FName>(Object, Name, Value); }

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Object", meta = (DisplayName = "Set Property (String)"))
    static bool SetProperty_String(UObject* Object, const FName Name, FString Value) { return SetProperty<UStrProperty, FString>(Object, Name, Value); }
	
	/*UFUNCTION(BlueprintCallable, Category = "EditorPlus|Object", meta = (DisplayName = "Set Property (Box 2D)"))
    static bool SetProperty_Box2D(UObject* Object, const FName Name, FBox2D Value) { return SetProperty<FBox2D>(Object, Name, Value); }*/

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Object", meta = (DisplayName = "Set Property (Vector)"))
    static bool SetProperty_Vector(UObject* Object, const FName Name, FVector Value) { return SetProperty<UStructProperty, FVector>(Object, Name, Value); }

	UFUNCTION(BlueprintCallable, Category = "EditorPlus|Object", meta = (DisplayName = "Set Property (Vector 2D)"))
    static bool SetProperty_Vector2D(UObject* Object, const FName Name, FVector2D Value) { return SetProperty<UStructProperty, FVector2D>(Object, Name, Value); }

	//UFUNCTION(BlueprintCallable, Category = "EditorPlus|Object", meta = (DisplayName = "Set Property (Primary Asset Id)"))
 //   static bool SetProperty_PrimaryAssetId(UObject* Object, const FName Name, FPrimaryAssetId Value) { return SetProperty(Object, Name, Value); }

private:
    static void* GetPropertyValuePtr(UProperty* Property, UProperty* Outer, void* Data, int32 ArrayIndex);

    template <typename PropertyType, typename ValueType>
    static bool SetProperty(UObject* Object, const FName Name, const ValueType& Value);

	template <typename PropertyType, typename ValueType>
    static bool SetPropertyValue(UObject* Object, PropertyType* Property, int32 ArrayIndex, const ValueType& Value);

    //template <typename PropertyType, typename ValueType>
    //static bool SetPropertyValue(UProperty* Outer, PropertyType* Property, int32 ArrayIndex, const ValueType& Value);
};
