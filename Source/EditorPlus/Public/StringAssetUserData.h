#pragma once

#include "Engine/AssetUserData.h"
#include "Templates/SubclassOf.h"
#include "JsonObjectConverter.h"

#include "StringAssetUserData.generated.h"

class FJsonObject;

UCLASS(BlueprintType, meta = (DisplayName = "String User Data"))
class EDITORPLUS_API UStringAssetUserData
    : public UAssetUserData
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, AdvancedDisplay, meta = (ScriptName = "Data", DisplayName = "Metadata"))
    FString DataString;

    TSharedPtr<FJsonObject> TryDeserialize();

    template <typename TStruct>
    TStruct TryDeserialize();

#if WITH_EDITORONLY_DATA
    virtual bool IsPostLoadThreadSafe() const override;
    virtual void PostLoad() override;
#endif
};

template <typename TStruct>
TStruct UStringAssetUserData::TryDeserialize()
{
    auto JsonObject = TryDeserialize();
    TStruct Result;
    FJsonObjectConverter::JsonObjectToUStruct(JsonObject, &Result);
    return Result;
}