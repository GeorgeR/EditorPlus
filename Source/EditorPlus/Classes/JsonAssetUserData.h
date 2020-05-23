#pragma once

#include "Engine/AssetUserData.h"
#include "Templates/SubclassOf.h"
#include "JsonObjectConverter.h"

#include "JsonAssetUserData.generated.h"

class FJsonObject;

UCLASS(BlueprintType, meta = (DisplayName = "Json User Data"))
class EDITORPLUS_API UJsonAssetUserData
    : public UAssetUserData
{
    GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
    virtual bool IsPostLoadThreadSafe() const override;
    virtual void PostLoad() override;
#endif

private:
	UPROPERTY()
    FString DataString;
};