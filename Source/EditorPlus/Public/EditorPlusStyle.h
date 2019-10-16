#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class EDITORPLUS_API FEditorPlusStyle
{
public:
	static void Initialize();
	static void Shutdown();

	static void ReloadTextures();

	static const ISlateStyle& Get();

	static FName GetStyleSetName();

private:
	static TSharedRef<class FSlateStyleSet> Create();
    static FString InContent(const FString& RelativePath, const ANSICHAR* Extension);

private:
	static TSharedPtr<class FSlateStyleSet> StyleInstance;
};