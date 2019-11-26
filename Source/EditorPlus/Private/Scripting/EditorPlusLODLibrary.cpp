#include "Scripting/EditorPlusLODLibrary.h"

#include "Modules/ModuleManager.h"
#include "HierarchicalLODUtilitiesModule.h"
#include "IHierarchicalLODUtilities.h"

bool UEditorPlusLODLibrary::ShouldGenerateCluster(AActor* InActor, const int32 HLODLevelIndex)
{
    FHierarchicalLODUtilitiesModule& Module = FModuleManager::LoadModuleChecked<FHierarchicalLODUtilitiesModule>("HierarchicalLODUtilities");
    IHierarchicalLODUtilities* Utilities = Module.GetUtilities();

    auto Result = Utilities->ShouldGenerateCluster(InActor, HLODLevelIndex);

    if(Result != EClusterGenerationError::None)
    {
        auto Message = TEXT("");
        // #todo: log specific error

        return false;
    }
    
    return true;
}