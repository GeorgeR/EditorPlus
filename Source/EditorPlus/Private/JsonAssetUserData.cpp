#include "JsonAssetUserData.h"

#include "Json.h"

#if WITH_EDITORONLY_DATA

bool UJsonAssetUserData::IsPostLoadThreadSafe() const
{
    return true;
}

void UJsonAssetUserData::PostLoad()
{
    Super::PostLoad();

    ClearFlags(RF_Transactional);
}

#endif
