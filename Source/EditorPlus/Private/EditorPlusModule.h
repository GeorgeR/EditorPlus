#pragma once

#include "IEditorPlus.h"

#include "CoreMinimal.h"

class FEditorPlusDetailCustomizations;

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
};