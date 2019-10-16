#include "Scripting/EditorPlusViewportLibrary.h"

#include "Editor.h"
#include "LevelEditorViewport.h"

int32 UEditorPlusViewportLibrary::GetActiveViewportIndex()
{
	auto Index = 1;
	if(GEditor && GCurrentLevelEditingViewportClient)
		GEditor->GetLevelViewportClients().Find(GCurrentLevelEditingViewportClient, Index);
	return Index;
}

void UEditorPlusViewportLibrary::SetViewportLocationAndRotation(const int32 ViewportIndex, const FVector Location, const FRotator Rotation)
{
	if(GEditor && ViewportIndex < GEditor->GetLevelViewportClients().Num())
	{
		auto LevelViewportClient = GEditor->GetLevelViewportClients()[ViewportIndex];
		if(LevelViewportClient)
		{
			LevelViewportClient->SetViewLocation(Location);
			LevelViewportClient->SetViewRotation(Rotation);
		}
	}
}
