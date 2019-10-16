#include "EditorPlusTaskSubsystem.h"

#include "Editor.h"
#include "FileHelpers.h"

void UEditorPlusTaskSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UEditorPlusTaskSubsystem::Deinitialize()
{
}

void UEditorPlusTaskSubsystem::ForEachLevel(const TArray<FString>& Levels, FEditorPlusForEachLevelDelegate Function)
{
	if(!Function.IsBound())
		return;

	const auto Subsystem = GetSelf();
	ensure(Subsystem);

	for(auto& Level : Levels)
	{
		if(UEditorLoadingAndSavingUtils::LoadMap(Level))
		{
			FEditorScriptExecutionGuard ScriptGuard;
			Function.Execute(Level);
			FEditorFileUtils::SaveDirtyPackages(false, true, true);
		}
	}
}

UEditorPlusTaskSubsystem* UEditorPlusTaskSubsystem::GetSelf()
{
	return GEditor->GetEditorSubsystem<UEditorPlusTaskSubsystem>();
}
