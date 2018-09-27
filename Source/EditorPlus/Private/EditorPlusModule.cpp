#include "EditorPlusModule.h"

//DEFINE_LOG_CATEGORY(LogEditorPlus);
DEFINE_LOG_CATEGORY(LogEditorPlus);

#define LOCTEXT_NAMESPACE "FEditorPlusModule"

void FEditorPlusModule::StartupModule() { }

void FEditorPlusModule::ShutdownModule() { }

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEditorPlusModule, EditorPlus)