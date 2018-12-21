#pragma once

#define DECLARE_MAKE_INSTANCE() static TSharedRef<IPropertyTypeCustomization> MakeInstance()
#define IMPLEMENT_MAKE_INSTANCE(Type) TSharedRef<IPropertyTypeCustomization> FNumericCustomization::MakeInstance()  \
    {   \
        return MakeShareable(new Type);    \
    }