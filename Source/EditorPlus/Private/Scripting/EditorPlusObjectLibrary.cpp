#include "Scripting/EditorPlusObjectLibrary.h"

void UEditorPlusObjectLibrary::AddToRoot(UObject* Object)
{
    check(Object);

    Object->AddToRoot();
}

void UEditorPlusObjectLibrary::RemoveFromRoot(UObject* Object)
{
    check(Object);

    Object->RemoveFromRoot();
}

void* UEditorPlusObjectLibrary::GetPropertyValuePtr(UProperty* Property, UProperty* Outer, void* Data, int32 ArrayIndex)
{
    check(Property);

    if(const auto ArrayProperty = Cast<UArrayProperty>(Outer))
    {
        if (ArrayProperty->Inner != Property)
            return nullptr;

        FScriptArrayHelper ArrayHelper(ArrayProperty, ArrayProperty->ContainerPtrToValuePtr<void>(Data));
        const auto Index = ArrayHelper.AddValue();

        return ArrayHelper.GetRawPtr(Index);
    }

    if (ArrayIndex >= Property->ArrayDim)
        return nullptr;

    return Property->ContainerPtrToValuePtr<void>(Data, ArrayIndex);
}

template <typename PropertyType, typename ValueType>
bool UEditorPlusObjectLibrary::SetProperty(UObject* Object, const FName Name, const ValueType& Value)
{
    check(Object);
    check(Name != NAME_None);

    const auto Class = Object->GetClass();
    const auto Property = Class->FindPropertyByName(Name);
    if(!Property)
    {
        UE_LOG(LogEditorPlus, Warning, TEXT("Failed to find property '%s' for object with class '%s'"), *Name.ToString(), *Class->GetName());
        return false;
    }

    auto* PropertyAddr = Property->ContainerPtrToValuePtr<void>(Object);
    if (PropertyAddr == nullptr)
        return false;

    if(!Object->HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
    {
        FEditPropertyChain PropertyChain;
        PropertyChain.AddHead(Property);
        Object->PreEditChange(PropertyChain);
    }

    {
    	SetPropertyValue<PropertyType, ValueType>(Object, Cast<PropertyType>(Property), 0, Value);
		//TFunction<void(UObject*, const ValueType&)> Setter = [this](UObject* Obj, const ValueType&)
    	
    	if(auto ArrayProperty = Cast<UArrayProperty>(Property))
    	{
    		
    	}
    	else if(auto StructProperty = Cast<UStructProperty>(Property))
    	{
    //		if(StructProperty->Struct->GetFName() == NAME_Vector)
				//StructProperty->
    		//StructProperty->
    	}
        //else if (auto FloatProperty = Cast<UFloatProperty>(Property))
        //    FloatProperty->SetFloatingPointPropertyValue(Property->ContainerPtrToValuePtr<float>(Object), Value);
        //else if (auto DoubleProperty = Cast<UDoubleProperty>(Property))
        //    DoubleProperty->SetPropertyValue(Property->ContainerPtrToValuePtr<double>(Object), Value);
        //else if (auto IntProperty = Cast<UIntProperty>(Property))
        //    IntProperty->SetPropertyValue(Property->ContainerPtrToValuePtr<int32>(Object), Value);
        //else if (auto BoolProperty = Cast<UBoolProperty>(Property))
        //    BoolProperty->SetPropertyValue(Property->ContainerPtrToValuePtr<void>(Object), Value);
		
        //else if (auto StrProperty = Cast<UStrProperty>(Property))
        //{
        //    FString ValueRef = Value;
        //    StrProperty->SetPropertyValue(Property->ContainerPtrToValuePtr<UObject*>(Object), ValueRef);
        //}
        //else if (auto TextProperty = Cast<UTextProperty>(Property))
        //    TextProperty->SetPropertyValue(Property->ContainerPtrToValuePtr<FText>(Object), Value);
        //else if (auto NameProperty = Cast<UNameProperty>(Property))
        //    NameProperty->SetPropertyValue(Property->ContainerPtrToValuePtr<FName>(Object), Value);
    }

    if(!Object->HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
    {
        FPropertyChangedEvent PropertyEvent(Property);
        Object->PostEditChangeProperty(PropertyEvent);
    }

    return true;
}

template <typename PropertyType, typename ValueType>
bool UEditorPlusObjectLibrary::SetPropertyValue(
    UObject* Object,
    PropertyType* Property,
    int32 ArrayIndex,
    const ValueType& Value)
{
	//unimplemented();

	ValueType* ValuePtr = nullptr;
	ValuePtr = Property->template ContainerPtrToValuePtr<ValueType>(Object);
	*ValuePtr = Value;
	
    return false;
}

template <>
bool UEditorPlusObjectLibrary::SetPropertyValue(
    UObject* Object,
    UByteProperty* Property,
    int32 ArrayIndex,
    const uint8& Value)
{
	Property->SetPropertyValue(Property->ContainerPtrToValuePtr<uint8>(Object), Value);
    return true;
}

template <>
bool UEditorPlusObjectLibrary::SetPropertyValue(
    UObject* Object,
    UInt8Property* Property,
    int32 ArrayIndex,
    const int8& Value)
{
	Property->SetPropertyValue(Property->ContainerPtrToValuePtr<int8>(Object), Value);
    return true;
}

template <>
bool UEditorPlusObjectLibrary::SetPropertyValue(
    UObject* Object,
    UInt16Property* Property,
    int32 ArrayIndex,
    const int16& Value)
{
	Property->SetPropertyValue(Property->ContainerPtrToValuePtr<int16>(Object), Value);
    return true;
}

template <>
bool UEditorPlusObjectLibrary::SetPropertyValue(
    UObject* Object,
    UIntProperty* Property,
    int32 ArrayIndex,
    const int32& Value)
{
	Property->SetPropertyValue(Property->ContainerPtrToValuePtr<int32>(Object), Value);
    return true;
}

template <>
bool UEditorPlusObjectLibrary::SetPropertyValue(
    UObject* Object,
    UInt64Property* Property,
    int32 ArrayIndex,
    const int64& Value)
{
	Property->SetPropertyValue(Property->ContainerPtrToValuePtr<int64>(Object), Value);
    return true;
}

template <>
bool UEditorPlusObjectLibrary::SetPropertyValue(
    UObject* Object,
    UUInt16Property* Property,
    int32 ArrayIndex,
    const uint16& Value)
{
	Property->SetPropertyValue(Property->ContainerPtrToValuePtr<uint16>(Object), Value);
    return true;
}

template <>
bool UEditorPlusObjectLibrary::SetPropertyValue(
    UObject* Object,
    UUInt32Property* Property,
    int32 ArrayIndex,
    const uint32& Value)
{
	Property->SetPropertyValue(Property->ContainerPtrToValuePtr<uint32>(Object), Value);
    return true;
}

template <>
bool UEditorPlusObjectLibrary::SetPropertyValue(
    UObject* Object,
    UUInt64Property* Property,
    int32 ArrayIndex,
    const uint64& Value)
{
	Property->SetPropertyValue(Property->ContainerPtrToValuePtr<uint64>(Object), Value);
    return true;
}

template <>
bool UEditorPlusObjectLibrary::SetPropertyValue(
    UObject* Object,
    UFloatProperty* Property,
    int32 ArrayIndex,
    const float& Value)
{
	Property->SetPropertyValue(Property->ContainerPtrToValuePtr<float>(Object), Value);
    return true;
}

template <>
bool UEditorPlusObjectLibrary::SetPropertyValue(
    UObject* Object,
    UDoubleProperty* Property,
    int32 ArrayIndex,
    const double& Value)
{
	Property->SetPropertyValue(Property->ContainerPtrToValuePtr<double>(Object), Value);
    return true;
}

template <>
bool UEditorPlusObjectLibrary::SetPropertyValue(
    UObject* Object,
    UBoolProperty* Property,
    int32 ArrayIndex,
    const bool& Value)
{
	Property->SetPropertyValue(Property->ContainerPtrToValuePtr<void>(Object), Value);
    return true;
}

template <>
bool UEditorPlusObjectLibrary::SetPropertyValue(
    UObject* Object,
    UNameProperty* Property,
    int32 ArrayIndex,
    const FName& Value)
{
	Property->SetPropertyValue(Property->ContainerPtrToValuePtr<FName>(Object), Value);
    return true;
}

template <>
bool UEditorPlusObjectLibrary::SetPropertyValue(
    UObject* Object,
    UStrProperty* Property,
    int32 ArrayIndex,
    const FString& Value)
{
	Property->SetPropertyValue(Property->ContainerPtrToValuePtr<FString>(Object), Value);
    return true;
}

//template <>
//bool UEditorPlusObjectLibrary::SetPropertyValue(
//    UObject* Object,
//    UStructProperty* Property,
//    int32 ArrayIndex,
//    const FVector& Value)
//{
//	check(Property->Struct->GetFName() == NAME_Vector);
//	auto* PropertyValue = Property->ContainerPtrToValuePtr<FVector>(Object);
//	PropertyValue->Set(Value.X, Value.Y, Value.Z);
//
//	//Property->SetPropertyValue(Property->ContainerPtrToValuePtr<FString>(Object), Value);
//    return true;
//}