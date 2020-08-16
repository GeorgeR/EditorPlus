#pragma once

#include "CoreMinimal.h"
#include "EditorValidatorBase.h"

#include "MetadataBasedValidator.generated.h"

UCLASS()
class EDITORPLUS_API UMetadataBasedValidator
	: public UEditorValidatorBase
{
	GENERATED_BODY()

public:
	bool CanValidateAsset_Implementation(UObject* InAsset) const override { return true; }
	
	EDataValidationResult ValidateLoadedAsset_Implementation(UObject* InAsset, TArray<FText>& ValidationErrors) override;

protected:
	#if ENGINE_MINOR_VERSION <= 24
	bool CheckRequired(UObject* Outer, UProperty* Property) const;
	#else
	bool CheckRequired(UObject* Outer, FProperty* Property) const;
	#endif
};
