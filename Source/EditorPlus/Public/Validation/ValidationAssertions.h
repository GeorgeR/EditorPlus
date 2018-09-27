#pragma once

#include "CoreMinimal.h"
#include <SoftObjectPtr.h>

namespace Validation
{
    class EDITORPLUS_API FUtilities
    {
    public:
        static bool PrintErrors(const struct FLogCategoryBase& LogCategory, TArray<FText>& ValidationErrors);
    };

    class EDITORPLUS_API FAssert
    {
    public:
        static bool That(bool Condition, TArray<FText>& ValidationErrors);
        static bool That(bool Condition, FText& Message, TArray<FText>& ValidationErrors);

        static bool IsDataValid(UObject* Object, TArray<FText>& ValidationErrors);

        template <typename T>
        static bool IsNotNull(T* Object, const FName ObjectName, TArray<FText>& ValidationErrors);

        template <class T = UObject>
        static bool IsNotNull(TSoftObjectPtr<T> Object, const FName ObjectName, TArray<FText>& ValidationErrors);

        template <typename T>
        static bool IsNotEmpty(TArray<T*> Array, const FName ArrayName, TArray<FText>& ValidationErrors);
    };

    template <typename T>
    bool Validation::FAssert::IsNotNull(T* Object, const FName ObjectName, TArray<FText>& ValidationErrors)
    {
        if (Object != nullptr)
            return true;

        ValidationErrors.Add(FText::FromString(FString::Printf(TEXT("The input object with name %s was null."), *ObjectName.ToString())));
        return false;
    }

    template <class T /*= UObject*/>
    bool Validation::FAssert::IsNotNull(TSoftObjectPtr<T> Object, const FName ObjectName, TArray<FText>& ValidationErrors)
    {
        if (Object.IsValid())
            return true;

        ValidationErrors.Add(FText::FromString(FString::Printf(TEXT("The input object with name %s was null."), *ObjectName.ToString())));
        return false;
    }

    template <typename T>
    bool Validation::FAssert::IsNotEmpty(TArray<T*> Array, const FName ArrayName, TArray<FText>& ValidationErrors)
    {
        if (Array.Num() > 0)
            return true;

        ValidationErrors.Add(FText::FromString(FString::Printf(TEXT("The input collection with name %s was null."), *ArrayName.ToString())));
        return false;
    }
}