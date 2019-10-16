#pragma once

#include "IEditorPlus.h"

#include "CoreMinimal.h"

class FEditorPlusDetailCustomizations;
class FEditorPlusToolbarExtender;

class FToolBarBuilder;
class FMenuBuilder;
class FUICommandList;

class FEditorPlusModule
    : public IEditorPlusModule
{
public:
    FEditorPlusModule();
    
	void StartupModule() override;
	void ShutdownModule() override;

    bool SupportsDynamicReloading() override { return true; }

private:
    TUniquePtr<FEditorPlusDetailCustomizations> DetailCustomizations;
	TUniquePtr<FEditorPlusToolbarExtender> ToolbarExtender;
};