#include "MetadataBasedValidator.h"

#define LOCTEXT_NAMESPACE "AssetValidation"

EDataValidationResult UMetadataBasedValidator::ValidateLoadedAsset_Implementation(UObject* InAsset, TArray<FText>& ValidationErrors)
{
	check(InAsset);

	static FName NAME_Required = TEXT("Required");
	static FName NAME_Optional = TEXT("Optional");

	auto bHasFailedOnce = false;

	const auto Class = InAsset->GetClass();
	for(TFieldIterator<FProperty> Iterator(Class); Iterator; ++Iterator)
	{
		auto* Property = *Iterator;
		
		if(Property->HasMetaData(NAME_Required))
		{
			if(!CheckRequired(InAsset, Property))
			{
				auto Message = FText::FromString(FString::Printf(TEXT("Property '%s' is Required but isn't valid."), *Property->GetNameCPP()));
				AssetFails(InAsset, Message, ValidationErrors);
				bHasFailedOnce = true;
			}
		}		
	}

	if(!bHasFailedOnce)
		AssetPasses(InAsset);

	return GetValidationResult();
}

bool UMetadataBasedValidator::CheckRequired(UObject* Outer, FProperty* Property) const
{
	if(const auto SoftObjectProperty = CastFieldChecked<FSoftObjectProperty>(Property))
	{
		const auto Value = SoftObjectProperty->GetPropertyValue_InContainer(Outer);
		if(Value.IsNull())
			return false;
	}
	
	return true;
}

#undef LOCTEXT_NAMESPACE
