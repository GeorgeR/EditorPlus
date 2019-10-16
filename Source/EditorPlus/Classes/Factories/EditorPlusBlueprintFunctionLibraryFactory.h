#pragma once

#include "CoreMinimal.h"
#include "Factories/BlueprintFunctionLibraryFactory.h"

#include "EditorPlusBlueprintFunctionLibraryFactory.generated.h"

UCLASS(MinimalAPI, HideCategories=Object, CollapseCategories)
class UEditorPlusBlueprintFunctionLibraryFactory
	: public UBlueprintFunctionLibraryFactory
{
	GENERATED_BODY()

public:
	UEditorPlusBlueprintFunctionLibraryFactory();

	// UFactory interface
	virtual FText GetDisplayName() const override;
	virtual FName GetNewAssetThumbnailOverride() const override;
	virtual uint32 GetMenuCategories() const override;
	virtual FText GetToolTip() const override;
	virtual FString GetToolTipDocumentationExcerpt() const override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual bool ConfigureProperties() override;
	virtual FString GetDefaultNewAssetName() const override;
	// End of UFactory interface
};
