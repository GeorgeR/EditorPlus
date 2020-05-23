#include "Scripting/EditorPlusFileLibrary.h"

bool UEditorPlusFileLibrary::ReadTextFile(const FString& Path, FString& OutResult)
{
	check(!Path.IsEmpty());

	// @todo: log
	if(!FPaths::FileExists(Path))
		return false;
	
	return FFileHelper::LoadFileToString(OutResult, *Path);
}
