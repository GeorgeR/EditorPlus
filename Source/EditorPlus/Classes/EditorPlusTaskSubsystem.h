#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "UObject/NoExportTypes.h"

#include "EditorPlusTaskSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FEditorPlusForEachLevelDelegate, FString, LevelPath);

UCLASS()
class EDITORPLUS_API UEditorPlusTaskSubsystem
	: public UEditorSubsystem
{
	GENERATED_BODY()

public:
	// Begin USubsystem
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;
	// End USubsystem

public:
	UFUNCTION(BlueprintCallable, Category = "EditorPlus")
	static void ForEachLevel(const TArray<FString>& Levels, FEditorPlusForEachLevelDelegate Function);

private:
	static UEditorPlusTaskSubsystem* GetSelf();
};