#include "Factories/EditorPlusBlueprintFunctionLibraryFactory.h"

#include "Engine/Blueprint.h"
#include "AssetTypeCategories.h"

#include "EditorPlusBlueprintFunctionLibrary.h"
#include "Kismet2/KismetEditorUtilities.h"

#define LOCTEXT_NAMESPACE "EditorPlusFactories"

UEditorPlusBlueprintFunctionLibraryFactory::UEditorPlusBlueprintFunctionLibraryFactory()
{
	static FBoolConfigValueHelper CanCreateNewHelper(TEXT("CustomBlueprintFunctionLibrary"), TEXT("bCanCreateNew"));
	bCreateNew = CanCreateNewHelper;
	bEditAfterNew = true;
	SupportedClass = UBlueprint::StaticClass();
	ParentClass = UEditorPlusBlueprintFunctionLibrary::StaticClass();
}

FText UEditorPlusBlueprintFunctionLibraryFactory::GetDisplayName() const
{
	return LOCTEXT("EditorPlusBlueprintFunctionLibraryFactoryDescription", "Editor Blueprint Function Library");
}

FName UEditorPlusBlueprintFunctionLibraryFactory::GetNewAssetThumbnailOverride() const
{
	return TEXT("ClassThumbnail.BlueprintFunctionLibrary");
}

uint32 UEditorPlusBlueprintFunctionLibraryFactory::GetMenuCategories() const
{
	return EAssetTypeCategories::Blueprint;
}

FText UEditorPlusBlueprintFunctionLibraryFactory::GetToolTip() const
{
	return LOCTEXT("BlueprintFunctionLibraryTooltip", "Blueprint Function Libraries are containers of functions to be used in other blueprints. They cannot contain variables, inherit from other blueprints, or be placed in levels. Changes to functions in a Blueprint Function Library will take effect without recompiling the client blueprints.");
}

FString UEditorPlusBlueprintFunctionLibraryFactory::GetToolTipDocumentationExcerpt() const
{
	return TEXT("UBlueprint_FunctionLibrary");
}

UObject* UEditorPlusBlueprintFunctionLibraryFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	// Make sure we are trying to factory a blueprint, then create and init one
	check(Class->IsChildOf(UBlueprint::StaticClass()));

	if (ParentClass != UEditorPlusBlueprintFunctionLibrary::StaticClass())
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("ClassName"), (ParentClass != nullptr) ? FText::FromString(ParentClass->GetName()) : LOCTEXT("Null", "(null)"));
		FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("CannotCreateBlueprintFromClass", "Cannot create a blueprint based on the class '{0}'."), Args));
		return nullptr;
	}
	else
	{
		return FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, Name, BPTYPE_FunctionLibrary, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass(), CallingContext);
	}
}

bool UEditorPlusBlueprintFunctionLibraryFactory::ConfigureProperties()
{
	return true;
}

FString UEditorPlusBlueprintFunctionLibraryFactory::GetDefaultNewAssetName() const
{
	return FString(TEXT("NewFunctionLibrary"));
}

#undef LOCTEXT_NAMESPACE
