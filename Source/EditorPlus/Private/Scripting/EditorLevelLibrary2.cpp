#include "EditorLevelLibrary2.h"
#include "EditorActorFolders.h"
#include "Editor.h"
#include "FileHelpers.h"

UWorld* GetEditorWorld()
{
    return GEditor ? GEditor->GetEditorWorldContext(false).World() : nullptr;
}

/* Return true if it exists, false if it didn't and was created */
bool UEditorLevelLibrary2::GetOrCreateFolder(FName Path)
{
    auto World = GetEditorWorld();
    check(World);

    auto& ActorFolders = FActorFolders::Get();
    /*auto& Folders = ActorFolders.GetOrCreateFoldersForWorld(World);
    if (Folders.Folders.Contains(Path))
        return true;
    else*/
    ActorFolders.CreateFolder(*World, Path);

    //UStaticMesh* SM;
    //SM->GetAssetUserDataOfClass()
    //SM->GetAssetUserDataOfClass()

    return true;
}

bool UEditorLevelLibrary2::MoveActorsToFolder(TArray<AActor*> Actors, FName Path)
{
    GetOrCreateFolder(Path);

    auto World = GetEditorWorld();
    for (auto Actor : Actors)
        Actor->SetFolderPath_Recursively(Path);
    
    return true;
}

FString UEditorLevelLibrary2::CurrentLevelName()
{
    auto World = GetEditorWorld();
    check(World);

    FString Result = TEXT("World'");
    Result = Result.Append(World->GetFullName(nullptr).RightChop(6));
    Result = Result.Append("'");
    return Result;
}

void UEditorLevelLibrary2::SetWorldOrigin(const int32& X, const int32& Y, const int32& Z)
{
    auto World = GetEditorWorld();
    check(World);

    World->SetNewWorldOrigin(FIntVector(X, Y, Z));
}