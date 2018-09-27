#pragma once

#include "CoreMinimal.h"
#include "IEditorPlus.h"

class FEditorPlusModule 
    : public IEditorPlusModule
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};