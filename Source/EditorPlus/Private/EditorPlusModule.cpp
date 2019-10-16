#include "EditorPlusModule.h"

#include "ModuleManager.h"

#include "DetailCustomizations/EditorPlusDetailCustomizations.h"
#include "UI/EditorPlusToolbarExtender.h"
#include "EditorPlusStyle.h"
#include "EditorPlusCommands.h"

DEFINE_LOG_CATEGORY(LogEditorPlus);

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