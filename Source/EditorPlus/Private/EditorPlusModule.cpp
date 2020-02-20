#include "EditorPlusModule.h"

#include "ModuleManager.h"

#include "DetailCustomizations/EditorPlusDetailCustomizations.h"
#include "UI/EditorPlusToolbarExtender.h"
#include "EditorPlusStyle.h"
#include "EditorPlusCommands.h"

#include "EditorValidatorSubsystem.h"
#include "Validation/MetadataBasedValidator.h"

#define LOCTEXT_NAMESPACE "EditorPlus"

FEditorPlusModule::FEditorPlusModule()
{
    DetailCustomizations = MakeUnique<FEditorPlusDetailCustomizations>();
	ToolbarExtender = MakeUnique<FEditorPlusToolbarExtender>();
}

void FEditorPlusModule::StartupModule()
{
    FEditorPlusStyle::Initialize();
    FEditorPlusCommands::Register();

	DetailCustomizations->Startup();
	ToolbarExtender->Extend();

	auto* ValidatorSubsystem = GEditor->GetEditorSubsystem<UEditorValidatorSubsystem>();
	if(ValidatorSubsystem)
	{
		const auto MetadataValidator = NewObject<UMetadataBasedValidator>(ValidatorSubsystem);
		ValidatorSubsystem->AddValidator(MetadataValidator);
	}
}

void FEditorPlusModule::ShutdownModule() 
{
	ToolbarExtender->Release();
    DetailCustomizations->Shutdown();

    FEditorPlusCommands::Unregister();
	FEditorPlusStyle::Shutdown();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEditorPlusModule, EditorPlus)
