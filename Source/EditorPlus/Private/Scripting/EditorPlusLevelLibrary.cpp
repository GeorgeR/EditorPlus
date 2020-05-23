#include "Scripting/EditorPlusLevelLibrary.h"

#include "EditorActorFolders.h"
#include "Editor.h"
#include "FileHelpers.h"
#include "EditorLevelUtils.h"
#include "Engine/LevelStreamingAlwaysLoaded.h"
#include "Engine/World.h"
#include "Engine/WorldComposition.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/LevelStreamingDynamic.h"
//#include "EditorObjectLibrary.h"
#include "WorldBrowserModule.h"
#include "HierarchicalLOD.h"
#include "EngineUtils.h"
#include "Engine/LODActor.h"

#include "EditorPlusInternalUtilties.h"

// @note these are private
#include "LevelCollectionModel.h"
#include "LevelModel.h"
#include "Tiles/WorldTileModel.h"
#include "Tiles/WorldTileDetails.h"

#define LOCTEXT_NAMESPACE "EditorPlus"

void UEditorPlusLevelLibrary::BuildHLOD(bool bForceRebuild)
{
	const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();

	auto HLODBuilder = World->HierarchicalLODBuilder;
	if(HLODBuilder->NeedsBuild(true) || bForceRebuild)
	{
		DeselectAllActorsOfClass(ALODActor::StaticClass());
		
		HLODBuilder->Build();
		HLODBuilder->BuildMeshesForLODActors(bForceRebuild);
	}
}

void UEditorPlusLevelLibrary::BuildHLODClusters(const bool bRebuild)
{
	const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();
	auto HLODBuilder = World->HierarchicalLODBuilder;

	TArray<ALODActor*> ActorsToBuild;
	for(TActorIterator<ALODActor> ActorIterator(World); ActorIterator; ++ActorIterator)
	{
		const auto LODActor = *ActorIterator;
		if(!LODActor->IsBuilt(bRebuild))
			ActorsToBuild.Add(LODActor);
	}

	FScopedSlowTask SlowTask(ActorsToBuild.Num(), FText::Format( LOCTEXT("EditorPlus.BuildClusters", "Building Cluster {0}|plural(one=Mesh,other=Meshes)"), ActorsToBuild.Num()));
	SlowTask.MakeDialog();
	{
		for(auto& LODActor : ActorsToBuild)
		{
			//GEditor->MoveViewportCamerasToActor(*LODActor, false);
			auto Bounds = LODActor->GetComponentsBoundingBox(true);
			GEditor->MoveViewportCamerasToBox(Bounds, false);
			GEditor->RedrawLevelEditingViewports();
			
			HLODBuilder->BuildMeshForLODActor(LODActor, LODActor->LODLevel - 1);
			
			SlowTask.EnterProgressFrame();
		}
	}
}

bool UEditorPlusLevelLibrary::IsHLODBuilt()
{
	const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();

	const auto HLODBuilder = World->HierarchicalLODBuilder;
	return HLODBuilder->NeedsBuild(true);
}

bool UEditorPlusLevelLibrary::DeselectAllActorsOfClass(const TSubclassOf<AActor> ActorClass)
{
	auto bSomethingWasDeselected = false;
	
	const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();
	for(TActorIterator<AActor> ActorIterator(World, ActorClass); ActorIterator; ++ActorIterator)
	{
		const auto Actor = *ActorIterator;
		GEditor->SelectActor(Actor, false, false, true);
		bSomethingWasDeselected = true;
	}

	return bSomethingWasDeselected;
}

/* Return true if it exists, false if it didn't and was created */
bool UEditorPlusLevelLibrary::GetOrCreateFolder(FName Path)
{
	const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();

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

bool UEditorPlusLevelLibrary::MoveActorsToFolder(TArray<AActor*> Actors, const FName Path)
{
    GetOrCreateFolder(Path);

    const auto World = FEditorPlusInternalUtilities::GetEditorWorld();
    for (auto Actor : Actors)
        Actor->SetFolderPath_Recursively(Path);

    return true;
}

FString UEditorPlusLevelLibrary::CurrentLevelName()
{
	const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();

	return World->GetName();
}

FString UEditorPlusLevelLibrary::GetCurrentLevelPath()
{
    const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();

    FString Result = TEXT("World'");
    Result = Result.Append(World->GetFullName(nullptr).RightChop(6));
    Result = Result.Append("'");
    return Result;
}

void UEditorPlusLevelLibrary::SetWorldOrigin(const int32& X, const int32& Y, const int32& Z)
{
    const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();

    World->SetNewWorldOrigin(FIntVector(X, Y, Z));
}

void UEditorPlusLevelLibrary::AddStreamingLevel(const FName& Path)
{
	const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();
	UEditorLevelUtils::AddLevelToWorld(World, *Path.ToString(), ULevelStreamingAlwaysLoaded::StaticClass());
}

TArray<AActor*> UEditorPlusLevelLibrary::GetActorsInSublevel(const FName& SublevelPath)
{
	TArray<AActor*> Result;

	return Result;
}

TArray<FString> UEditorPlusLevelLibrary::GetSubLevels()
{
	const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();

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
TArray<ULevel*> UEditorPlusLevelLibrary::LoadSubLevels(const TArray<FString>& Paths)
{
	const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();

	auto TaskLabel = FText::FromString(TEXT("Loading Levels"));
	GWarn->BeginSlowTask(TaskLabel, true);

	TArray<ULevel*> Levels;
	auto LevelIndex = 0;
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

ULevel* UEditorPlusLevelLibrary::LoadSubLevel(const FString& Path)
{
	const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();

	auto PackageName = FName(*Path);

	/* Get Streaming Level */
	ULevelStreaming* AssociatedStreamingLevel = nullptr;
	{
		// Try to find existing object first
		const auto FindStreamingLevel = [&](ULevelStreaming* StreamingLevel)
		{
			return (StreamingLevel && StreamingLevel->GetWorldAssetPackageFName() == PackageName && StreamingLevel->HasAnyFlags(RF_Transient));
		};

		if (const auto FoundStreamingLevel = World->GetStreamingLevels().FindByPredicate(FindStreamingLevel))
			AssociatedStreamingLevel = *FoundStreamingLevel;

		if (!AssociatedStreamingLevel)
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
	if (AssociatedStreamingLevel->GetLoadedLevel())
		return AssociatedStreamingLevel->GetLoadedLevel();

	// Load level package 
	{
		const auto LevelPackageName = AssociatedStreamingLevel->GetWorldAssetPackageFName();

		ULevel::StreamedLevelsOwningWorld.Add(LevelPackageName, World);
		UWorld::WorldTypePreLoadMap.FindOrAdd(LevelPackageName) = World->WorldType;

		const auto LevelPackage = LoadPackage(nullptr, *LevelPackageName.ToString(), LOAD_None);

		ULevel::StreamedLevelsOwningWorld.Remove(LevelPackageName);
		UWorld::WorldTypePreLoadMap.Remove(LevelPackageName);

		// Find world object and use its PersistentLevel pointer.
		auto LevelWorld = UWorld::FindWorldInPackage(LevelPackage);
		
		// Check for a redirector. Follow it, if found.
		if (!LevelWorld)
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

	const auto LoadedLevel = AssociatedStreamingLevel->GetLoadedLevel();

	FUnmodifiableObject ImmuneLevel(LoadedLevel);
	EditorLevelUtils::SetLevelVisibility(LoadedLevel, true, true);

	return LoadedLevel;
}

void UEditorPlusLevelLibrary::LoadSubLevelAsync(const FString& Path)
{
	const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();

	auto PackageName = FName(*Path);

	/* Get Streaming Level */
	ULevelStreaming* AssociatedStreamingLevel = nullptr;
	{
		// Try to find existing object first
		const auto FindStreamingLevel = [&](ULevelStreaming* StreamingLevel)
		{
			return (StreamingLevel && StreamingLevel->GetWorldAssetPackageFName() == PackageName && StreamingLevel->HasAnyFlags(RF_Transient));
		};

		if (const auto FoundStreamingLevel = World->GetStreamingLevels().FindByPredicate(FindStreamingLevel))
			AssociatedStreamingLevel = *FoundStreamingLevel;

		if (!AssociatedStreamingLevel)
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
	if (AssociatedStreamingLevel->GetLoadedLevel())
		return;

	// Load level package 
	{
		const auto LevelPackageName = AssociatedStreamingLevel->GetWorldAssetPackageFName();

		ULevel::StreamedLevelsOwningWorld.Add(LevelPackageName, World);
		UWorld::WorldTypePreLoadMap.FindOrAdd(LevelPackageName) = World->WorldType;

		//const auto LevelPackage = LoadPackage(nullptr, *LevelPackageName.ToString(), LOAD_None);
		auto OnLoaded = [&, World, LevelPackageName, AssociatedStreamingLevel](const FName& _PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type _Result)
		{
			check(IsInGameThread());
			if(!AssociatedStreamingLevel || !AssociatedStreamingLevel->IsValidLowLevel())
				return;
			
			ULevel::StreamedLevelsOwningWorld.Remove(LevelPackageName);
			UWorld::WorldTypePreLoadMap.Remove(LevelPackageName);

			// Find world object and use its PersistentLevel pointer.
			auto LevelWorld = UWorld::FindWorldInPackage(LoadedPackage);
			
			// Check for a redirector. Follow it, if found.
			if (!LevelWorld)
				LevelWorld = UWorld::FollowWorldRedirectorInPackage(LoadedPackage);

			if (LevelWorld && LevelWorld->PersistentLevel)
			{
				// LevelStreaming is transient object so world composition stores color in ULevel object
				AssociatedStreamingLevel->LevelColor = LevelWorld->PersistentLevel->LevelColor;
			}

			// Our level package should be loaded at this point, so level streaming will find it in memory
			AssociatedStreamingLevel->SetShouldBeLoaded(true);
			AssociatedStreamingLevel->SetShouldBeVisible(false); // Should be always false in the Editor
			AssociatedStreamingLevel->SetShouldBeVisibleInEditor(false);
			World->FlushLevelStreaming();

			const auto LoadedLevel = AssociatedStreamingLevel->GetLoadedLevel();

			FUnmodifiableObject ImmuneLevel(LoadedLevel);
			EditorLevelUtils::SetLevelVisibility(LoadedLevel, true, true);
		};
		
		LoadPackageAsync(LevelPackageName.ToString(), FLoadPackageAsyncDelegate::CreateLambda(OnLoaded));
		//const auto LevelPackage = LoadPackageAsync()
	}
}

// Based on LevelCollectionModel::UnloadLevels
void UEditorPlusLevelLibrary::UnloadSubLevels(const TArray<ULevel*>& Levels, bool bSaveIfDirty)
{
	const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();

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

TArray<FIntPoint> UEditorPlusLevelLibrary::GetSubLevelPositions(const TArray<ULevel*>& Levels)
{
	const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();

	TArray<FIntPoint> Result;
	return Result;
}

FIntPoint UEditorPlusLevelLibrary::GetSubLevelPosition(ULevel* Level)
{
	const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();

	// if bounds is valid
	// local position from bounds center
	// level offset = absolute position
	// return localposition + (leveloffset - GetWorldOriginLocationXY(world))

	return FIntPoint();
}

void UEditorPlusLevelLibrary::SetSubLevelPositions(const TArray<ULevel*>& Levels, const FIntPoint& Position)
{
	const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();
}

void UEditorPlusLevelLibrary::SetSubLevelPosition(ULevel* Levels, const FIntPoint& Position)
{
	const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();
}

void UEditorPlusLevelLibrary::Test()
{

}

FIntPoint UEditorPlusLevelLibrary::GetWorldSize()
{
    return DoForWorldModel<FIntPoint>([](TSharedPtr<FLevelCollectionModel> WorldModel) { return WorldModel->GetWorldSize(); });
}

void UEditorPlusLevelLibrary::LoadLevel(const FString& Level)
{
	TArray<FString> Levels;
	Levels.Add(Level);

	return LoadLevels(Levels);
}

void UEditorPlusLevelLibrary::LoadLevels(const TArray<FString>& Levels)
{
	auto LevelModels = ToLevelModelList(Levels);
	if (LevelModels.Num() <= 0)
		return;

	DoForWorldModel([&](TSharedPtr<FLevelCollectionModel> WorldModel)
	{
		WorldModel->LoadLevels(LevelModels);
	});
}

void UEditorPlusLevelLibrary::UnloadLevels(const TArray<FString>& Levels)
{
	auto LevelModels = ToLevelModelList(Levels);
	if (LevelModels.Num() <= 0)
		return;

	DoForWorldModel([&](TSharedPtr<FLevelCollectionModel> WorldModel)
	{
		WorldModel->SaveLevels(LevelModels);
		WorldModel->UnloadLevels(LevelModels);
	});
}

const TArray<FString> UEditorPlusLevelLibrary::GetAllLevels()
{
    return DoForWorldModel<const TArray<FString>>([](TSharedPtr<FLevelCollectionModel> WorldModel) { return FromLevelModelList(WorldModel->GetAllLevels()); });
}

const TArray<FString> UEditorPlusLevelLibrary::GetSelectedLevels()
{
    return DoForWorldModel<const TArray<FString>>([](TSharedPtr<FLevelCollectionModel> WorldModel) { return FromLevelModelList(WorldModel->GetSelectedLevels()); });
}

FIntVector UEditorPlusLevelLibrary::GetLevelLocation(const FString& Level)
{
	TArray<FString> Levels;
	Levels.Add(Level);

	auto LevelModels = ToLevelModelList(Levels);
	if (LevelModels.Num() <= 0)
		return FIntVector::ZeroValue;

	const auto TileModel = StaticCastSharedPtr<FWorldTileModel>(LevelModels[0]);
	if (TileModel.IsValid())
	{
		if (TileModel->TileDetails != nullptr)
			return TileModel->TileDetails->Position;
	}

	return FIntVector::ZeroValue;
}

void UEditorPlusLevelLibrary::SetLevelLocation(const FString& Level, const FIntVector& Location)
{
	TArray<FString> Levels;
	Levels.Add(Level);

	auto LevelModels = ToLevelModelList(Levels);
	if (LevelModels.Num() <= 0)
		return;

	auto TileModel = StaticCastSharedPtr<FWorldTileModel>(LevelModels[0]);
	if (TileModel.IsValid())
	{
		if (TileModel->TileDetails != nullptr)
		{
			TileModel->TileDetails->Position = Location;
			TileModel->TileDetails->PositionChangedEvent.Broadcast();
		}
	}
}

bool UEditorPlusLevelLibrary::IsThreadProcessingTasks()
{
	const auto bIsProcessingTasks = FTaskGraphInterface::Get().IsThreadProcessingTasks(ENamedThreads::GetRenderThread_Local());
	return bIsProcessingTasks;
}

void UEditorPlusLevelLibrary::DoForWorldModel(TFunction<void(TSharedPtr<FLevelCollectionModel>)> Func)
{
	const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();

	auto& WorldBrowserModule = FModuleManager::LoadModuleChecked<FWorldBrowserModule>(TEXT("WorldBrowser"));
	const auto WorldModel = WorldBrowserModule.SharedWorldModel(World);
	ensure(WorldModel.IsValid());
	Func(WorldModel);
}

template <typename T>
T UEditorPlusLevelLibrary::DoForWorldModel(TFunction<T(TSharedPtr<FLevelCollectionModel>)> Func)
{
    const auto World = FEditorPlusInternalUtilities::GetEditorWorldChecked();

    auto& WorldBrowserModule = FModuleManager::LoadModuleChecked<FWorldBrowserModule>(TEXT("WorldBrowser"));
    auto WorldModel = WorldBrowserModule.SharedWorldModel(World);
    ensure(WorldModel.IsValid());
    return Func(WorldModel);
}

FLevelModelList UEditorPlusLevelLibrary::ToLevelModelList(const TArray<FString>& List)
{
    FLevelModelList Result;
    if (List.Num() == 0)
        return Result;

    auto AllLevels = DoForWorldModel<const FLevelModelList>([](TSharedPtr<FLevelCollectionModel> WorldModel) { return WorldModel->GetAllLevels(); });
    for (auto& Item : List)
    {
	    const auto Level = AllLevels.FindByPredicate([&](TSharedPtr<FLevelModel> LevelModel) {
            return LevelModel->GetPackageFileName().Equals(Item);
        });

        if (Level != nullptr)
            Result.Add(*Level);
    }
    return Result;
}

TArray<FString> UEditorPlusLevelLibrary::FromLevelModelList(const FLevelModelList& List)
{
    TArray<FString> Result;
    if (List.Num() == 0)
        return Result;

    for (auto& Item : List)
        Result.Add(Item->GetPackageFileName());

    return Result;
}

#undef LOCTEXT_NAMESPACE
