#include "EditorAssetLibrary2.h"
#include "Interfaces/Interface_AssetUserData.h"
#include "StringAssetUserData.h"

UStringAssetUserData* UEditorAssetLibrary2::GetOrAddStringUserData(UObject* Object)
{
    check(Object);

    if (IInterface_AssetUserData* AssetUserData = Cast<IInterface_AssetUserData>(Object))
    {
        auto Result = Cast<UStringAssetUserData>(AssetUserData->GetAssetUserDataOfClass(UStringAssetUserData::StaticClass()));
        if (Result == nullptr)
        {
            Result = NewObject<UStringAssetUserData>();
            AssetUserData->AddAssetUserData(Result);
        }

        return Result;
    }

    return nullptr;
}