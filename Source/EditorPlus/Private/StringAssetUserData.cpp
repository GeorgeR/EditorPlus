#include "StringAssetUserData.h"
#include "JsonReader.h"
#include "JsonSerializer.h"

TSharedPtr<FJsonObject> UStringAssetUserData::TryDeserialize()
{
    if (DataString.IsEmpty())
        return nullptr;

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(DataString);
    FJsonSerializer::Deserialize(JsonReader, JsonObject);

    return JsonObject;
}

#if WITH_EDITORONLY_DATA

bool UStringAssetUserData::IsPostLoadThreadSafe() const
{
    return true;
}

void UStringAssetUserData::PostLoad()
{
    Super::PostLoad();

    ClearFlags(RF_Transactional);
}

#endif