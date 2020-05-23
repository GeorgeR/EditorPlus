#include "JsonAssetUserData.h"

#include "JsonReader.h"
#include "JsonSerializer.h"

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