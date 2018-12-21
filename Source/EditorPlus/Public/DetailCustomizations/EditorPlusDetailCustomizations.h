#pragma once

#include "CoreMinimal.h"
#include "PropertyEditorDelegates.h"

class FEditorPlusDetailCustomizations
{
public:
    void Startup();
    void Shutdown();

private:
    void RegisterObjectCustomizations();
    void RegisterPropertyTypeCustomizations();

    void RegisterCustomClassLayout(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate);
    void RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate);

private:
    TSet<FName> RegisteredClassNames;
    TSet<FName> RegisteredPropertyTypeNames;
};