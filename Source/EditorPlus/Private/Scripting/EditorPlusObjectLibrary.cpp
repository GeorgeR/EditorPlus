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

template <typename ValueType>
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
        if (auto FloatProperty = Cast<UFloatProperty>(Property))
            FloatProperty->SetFloatingPointPropertyValue(Property->ContainerPtrToValuePtr<float>(Object), Value);
        else if (auto DoubleProperty = Cast<UDoubleProperty>(Property))
            DoubleProperty->SetPropertyValue(Property->ContainerPtrToValuePtr<double>(Object), Value);
        else if (auto IntProperty = Cast<UIntProperty>(Property))
            IntProperty->SetPropertyValue(Property->ContainerPtrToValuePtr<int32>(Object), Value);
        else if (auto BoolProperty = Cast<UBoolProperty>(Property))
            BoolProperty->SetPropertyValue(Property->ContainerPtrToValuePtr<void>(Object), Value);
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
    //if(void* Ptr = GetPropertyValuePtr(Property, Outer, Data, ArrayIndex))
    //{
    //    *(ValueType)Ptr = Value;
    //    return true;
    //}

    return false;
}
