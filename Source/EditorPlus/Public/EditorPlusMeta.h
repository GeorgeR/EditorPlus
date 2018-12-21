#pragma once

#include "CoreMinimal.h"
#include "Class.h"

/* UPROPERTY */
/*
Normalize/Normalized
Angle
ShowWhen
HideWhen
EnabledWhen
DisableWhen
Show/Hide: support wildcard
CategoryOrder
Required/Optional
Interval
Filter (for objects or class selections)
Button
ButtonGroup
*/

// Describes a metadata type
struct EDITOR_PLUS FMetaDataDescriptor
{
public:
    const FName Key;
};

// Holds metadata for a field instance
struct EDITOR_PLUS FMetaDataInstance
{
public:
    /* Native Values */
    FString DisplayName;

public:
    FMetaDataInstance(UField* Field)
    {
        Resolve(Field);
    }

    static bool HasMetaData(UField* Field, const FName& Key)
    {
        return Field->HasMetaData(Key);
    }

    template <typename TType>
    static TType GetMetaDataValueAs(UField* Field, const FName& Key)
    {
        auto& MetadataValue = Field->GetMetaData(Key);
        
        TType Value;
        TTypeFromString<TType>::FromString(Value, *MetadataValue);

        return Value;
    }

protected:
    virtual FMetaDataInstance Resolve(UField* Field);
};

struct EDITOR_PLUS FNumericMetaDataInstance
    : public FMetaDataInstance
{
private:
    /* Native Raw Values */
    FString UIMin_Str;
    FString UIMax_Str;
    FString SliderExponent_Str;
    FString Delta_Str;
    FString ClampMin_Str;
    FString ClampMax_Str;

public:
    /* Native Values */
    uint8 ShiftMouseMovePixelPerDelta;
    bool bSupportsDynamicSliderMinValue;
    bool bSupportsDynamicSliderMaxValue;

public:

};