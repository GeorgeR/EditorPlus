#include "DetailCustomizations/EditorPlusDetailCustomizations.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"

#include "Vector2Customization.h"

#define LOCTEXT_NAMESPACE "FEditorPlusDetailCustomizations"

#define REGISTER_CLASS_CUSTOMIZATION(Name) RegisterCustomClassLayout(TEXT(#Name), FOnGetDetailCustomizationInstance::CreateStatic(&F##Name##Details::MakeInstance))
#define REGISTER_PROPERTY_CUSTOMIZATION(Name) RegisterCustomPropertyTypeLayout(TEXT(#Name), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&F##Name##Customization::MakeInstance))

void FEditorPlusDetailCustomizations::Startup()
{
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

    RegisterPropertyTypeCustomizations();
    RegisterObjectCustomizations();

    PropertyEditorModule.NotifyCustomizationModuleChanged();
}

void FEditorPlusDetailCustomizations::Shutdown()
{
    if (FModuleManager::Get().IsModuleLoaded(TEXT("PropertyEditor")))
    {
        FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

        for (auto& ClassName : RegisteredClassNames)
            PropertyEditorModule.UnregisterCustomClassLayout(ClassName);

        for (auto& PropertyTypeName : RegisteredPropertyTypeNames)
            PropertyEditorModule.UnregisterCustomPropertyTypeLayout(PropertyTypeName);

        PropertyEditorModule.NotifyCustomizationModuleChanged();
    }
}

void FEditorPlusDetailCustomizations::RegisterObjectCustomizations()
{

}

void FEditorPlusDetailCustomizations::RegisterPropertyTypeCustomizations()
{
	// @todo: the properties dont even appear with this
    //REGISTER_PROPERTY_CUSTOMIZATION(Vector2D);
}

void FEditorPlusDetailCustomizations::RegisterCustomClassLayout(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate)
{
    check(ClassName != NAME_None);

    RegisteredClassNames.Add(ClassName);

    static FName PropertyEditor(TEXT("PropertyEditor"));
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
    PropertyEditorModule.RegisterCustomClassLayout(ClassName, DetailLayoutDelegate);
}

void FEditorPlusDetailCustomizations::RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate)
{
    check(PropertyTypeName != NAME_None);

    RegisteredPropertyTypeNames.Add(PropertyTypeName);

    static FName PropertyEditor(TEXT("PropertyEditor"));
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
    PropertyEditorModule.RegisterCustomPropertyTypeLayout(PropertyTypeName, PropertyTypeLayoutDelegate);
}

#undef LOCTEXT_NAMESPACE
