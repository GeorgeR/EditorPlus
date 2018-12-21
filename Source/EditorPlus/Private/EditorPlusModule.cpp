#include "EditorPlusModule.h"

#include "ModuleManager.h"
#include "DetailCustomizations/EditorPlusDetailCustomizations.h"

DEFINE_LOG_CATEGORY(LogEditorPlus);

#define LOCTEXT_NAMESPACE "FEditorPlusModule"

FEditorPlusModule::FEditorPlusModule()
{
    DetailCustomizations = MakeUnique<FEditorPlusDetailCustomizations>();
}

void FEditorPlusModule::StartupModule() 
{
    DetailCustomizations->Startup();
}

void FEditorPlusModule::ShutdownModule() 
{
    DetailCustomizations->Shutdown();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEditorPlusModule, EditorPlus)