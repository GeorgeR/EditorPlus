#include "Vector2Customization.h"

#include "UObject/UnrealType.h"

TSharedRef<IPropertyTypeCustomization> FVector2DCustomization::MakeInstance()
{
    return MakeShareable(new FVector2DCustomization);
}

void FVector2DCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
}

void FVector2DCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
}