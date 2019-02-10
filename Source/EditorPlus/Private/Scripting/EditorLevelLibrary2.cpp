#include "EditorLevelLibrary2.h"
#include "EditorActorFolders.h"
#include "Editor.h"
#include "FileHelpers.h"
#include "EditorLevelUtils.h"
#include "Engine/LevelStreamingAlwaysLoaded.h"
#include "Engine/World.h"
#include "Engine/WorldComposition.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/LevelStreamingDynamic.h"
#include "EditorObjectLibrary.h"

#include "WorldBrowserModule.h"

#include "Private/LevelCollectionModel.h"

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

	return World->GetName();
}

FString UEditorLevelLibrary2::GetCurrentLevelPath()
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

void UEditorLevelLibrary2::AddStreamingLevel(const FName& Path)
{
	auto World = GetEditorWorld();
	UEditorLevelUtils::AddLevelToWorld(World, *Path.ToString(), ULevelStreamingAlwaysLoaded::StaticClass());
}

TArray<AActor*> UEditorLevelLibrary2::GetActorsInSublevel(const FName& SublevelPath)
{
	TArray<AActor*> Result;

	return Result;
}

TArray<FString> UEditorLevelLibrary2::GetSubLevels()
{
	auto World = GetEditorWorld();
	check(World);

	TArray<FString> Result;
	if (World->GetWorldSettings()->bEnableWorldComposition)
	{
		auto Tiles = World->WorldComposition->GetTilesList();
		for (auto& Tile : Tiles)
			Result.Add(Tile.PackageName.ToString());
	}
	else
	{

	}

	return Result;
}

// Based on LevelCollectionModel::LoadLevels
TArray<ULevel*> UEditorLevelLibrary2::LoadSubLevels(const TArray<FString>& Paths)
{
	auto World = GetEditorWorld();
	check(World);

	FText TaskLabel = FText::FromString(TEXT("Loading Levels"));
	GWarn->BeginSlowTask(TaskLabel, true);

	TArray<ULevel*> Levels;
	int LevelIndex = 0;
	for (auto& Path : Paths)
	{
		TaskLabel = FText::FromString(FString::Printf(TEXT("Loading: %s..."), *Path));
		GWarn->StatusUpdate(LevelIndex++, Paths.Num(), TaskLabel);

		auto Level = LoadSubLevel(Path);
		Levels.Add(Level);
	}
	
	GWarn->EndSlowTask();

	return Levels;
}

ULevel* UEditorLevelLibrary2::LoadSubLevel(const FString& Path)
{
	auto World = GetEditorWorld();
	check(World);

	FName PackageName = FName(*Path);

	/* Get Streaming Level */
	ULevelStreaming* AssociatedStreamingLevel = nullptr;
	{
		// Try to find existing object first
		auto Predicate = [&](ULevelStreaming* StreamingLevel)
		{
			return (StreamingLevel && StreamingLevel->GetWorldAssetPackageFName() == PackageName && StreamingLevel->HasAnyFlags(RF_Transient));
		};

		if (ULevelStreaming*const* FoundStreamingLevel = World->GetStreamingLevels().FindByPredicate(Predicate))
			AssociatedStreamingLevel = *FoundStreamingLevel;

		if (AssociatedStreamingLevel == nullptr)
		{
			// Create new streaming level
			AssociatedStreamingLevel = NewObject<ULevelStreamingDynamic>(World, NAME_None, RF_Transient);

			AssociatedStreamingLevel->SetWorldAssetByPackageName(PackageName);
			AssociatedStreamingLevel->LevelTransform = FTransform::Identity;
			AssociatedStreamingLevel->PackageNameToLoad = PackageName;

			World->AddStreamingLevel(AssociatedStreamingLevel);
		}
	}

	check(AssociatedStreamingLevel);

	// Check if already loaded
	if (AssociatedStreamingLevel->GetLoadedLevel() != nullptr)
		return AssociatedStreamingLevel->GetLoadedLevel();

	// Load level package 
	{
		FName LevelPackageName = AssociatedStreamingLevel->GetWorldAssetPackageFName();

		ULevel::StreamedLevelsOwningWorld.Add(LevelPackageName, World);
		UWorld::WorldTypePreLoadMap.FindOrAdd(LevelPackageName) = World->WorldType;

		UPackage* LevelPackage = LoadPackage(nullptr, *LevelPackageName.ToString(), LOAD_None);

		ULevel::StreamedLevelsOwningWorld.Remove(LevelPackageName);
		UWorld::WorldTypePreLoadMap.Remove(LevelPackageName);

		// Find world object and use its PersistentLevel pointer.
		UWorld* LevelWorld = UWorld::FindWorldInPackage(LevelPackage);
		// Check for a redirector. Follow it, if found.
		if (LevelWorld == nullptr)
			LevelWorld = UWorld::FollowWorldRedirectorInPackage(LevelPackage);

		if (LevelWorld && LevelWorld->PersistentLevel)
		{
			// LevelStreaming is transient object so world composition stores color in ULevel object
			AssociatedStreamingLevel->LevelColor = LevelWorld->PersistentLevel->LevelColor;
		}
	}

	// Our level package should be loaded at this point, so level streaming will find it in memory
	AssociatedStreamingLevel->SetShouldBeLoaded(true);
	AssociatedStreamingLevel->SetShouldBeVisible(false); // Should be always false in the Editor
	AssociatedStreamingLevel->SetShouldBeVisibleInEditor(false);
	World->FlushLevelStreaming();

	auto LoadedLevel = AssociatedStreamingLevel->GetLoadedLevel();

	FUnmodifiableObject ImmuneLevel(LoadedLevel);
	EditorLevelUtils::SetLevelVisibility(LoadedLevel, true, true);

	return LoadedLevel;
}

// Based on LevelCollectionModel::UnloadLevels
void UEditorLevelLibrary2::UnloadSubLevels(const TArray<ULevel*>& Levels, bool bSaveIfDirty)
{
	auto World = GetEditorWorld();
	check(World);

	if (Levels.Num() == 0)
		return;

	auto TaskCount = Levels.Num();
	FText TaskLabel = FText::FromString(TEXT("Unloading Levels"));
	GWarn->BeginSlowTask(TaskLabel, true);

	auto LevelIndex = 0;
	for (auto& Level : Levels)
	{
		TaskLabel = FText::FromString(FString::Printf(TEXT("Unloading: %s..."), *Level->GetName()));
		GWarn->StatusUpdate(LevelIndex++, TaskCount, TaskLabel);

		if(Level == World->PersistentLevel)
			continue;

		// Unselect all actors before removing the level
		// This avoids crashing in areas that rely on getting a selected actors level. The level will be invalid after its removed.
		for (auto ActorIterator = Level->Actors.CreateIterator(); ActorIterator; ++ActorIterator)
			GEditor->SelectActor((*ActorIterator), false, false);

		if(bSaveIfDirty && Level->GetOutermost()->IsDirty())
			FEditorFileUtils::SaveLevel(Level);

		// In case we have created temporary streaming level object for this sub-level - remove it before unloading sub-level
		{
			FName LevelPackageName = FName(*Level->GetFullName());
			auto Predicate = [&](ULevelStreaming* StreamingLevel)
			{
				return (StreamingLevel && StreamingLevel->GetWorldAssetPackageFName() == LevelPackageName && StreamingLevel->HasAnyFlags(RF_Transient));
			};

			if (ULevelStreaming*const* StreamingLevel = World->GetStreamingLevels().FindByPredicate(Predicate))
			{
				(*StreamingLevel)->MarkPendingKill();
				World->RemoveStreamingLevel(*StreamingLevel);
			}
		}

		// Unload sub-level
		{
			FUnmodifiableObject ImmuneWorld(World);
			EditorLevelUtils::RemoveLevelFromWorld(Level);
		}
	}

	GWarn->EndSlowTask();
	
	CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
}

TArray<FIntPoint> UEditorLevelLibrary2::GetSubLevelPositions(const TArray<ULevel*>& Levels)
{
	auto World = GetEditorWorld();
	check(World);

	TArray<FIntPoint> Result;
	return Result;
}

FIntPoint UEditorLevelLibrary2::GetSubLevelPosition(ULevel* Level)
{
	auto World = GetEditorWorld();
	check(World);

	// if bounds is valid
	// local position from bounds center
	// level offset = absolute position
	// return localposition + (leveloffset - GetWorldOriginLocationXY(world))

	return FIntPoint();
}

void UEditorLevelLibrary2::SetSubLevelPositions(const TArray<ULevel*>& Levels, const FIntPoint& Position)
{
	auto World = GetEditorWorld();
	check(World);
}

void UEditorLevelLibrary2::SetSubLevelPosition(ULevel* Levels, const FIntPoint& Position)
{
	auto World = GetEditorWorld();
	check(World);
}

void UEditorLevelLibrary2::Test()
{
	auto World = GetEditorWorld();
	check(World);

	FWorldBrowserModule& WorldBrowserModule = FModuleManager::LoadModuleChecked<FWorldBrowserModule>(TEXT("WorldBrowser"));
	auto WorldModel = WorldBrowserModule.SharedWorldModel(World);
	if (WorldModel.IsValid())
	{
		auto& Levels = WorldModel->GetAllLevels();

		auto o = 123;
	}
}
