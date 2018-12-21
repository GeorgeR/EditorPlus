// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Based on the original MathStructCustomizations

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "IPropertyTypeCustomization.h"
#include "PropertyHandle.h"
#include "DetailCustomizationMacros.h"

class FDetailWidgetRow;
class IDetailChildrenBuilder;

/**
 * Base class for numeric customization (e.g, vector, rotator, color)
 */
class FNumericCustomization
    : public IPropertyTypeCustomization
{
public:
    DECLARE_MAKE_INSTANCE();
    
    /** Notification when the max/min slider values are changed (only apply if SupportDynamicSliderMaxValue or SupportDynamicSliderMinValue are true) */
    DECLARE_MULTICAST_DELEGATE_FourParams(FOnNumericEntryBoxDynamicSliderMinMaxValueChanged, float, TWeakPtr<SWidget>, bool, bool);

    FNumericCustomization()
        : bIsUsingSlider(false)
        , bPreserveScaleRatio(false)
    { }

    virtual ~FNumericCustomization() {}

    /** IPropertyTypeCustomization instance */
    virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
    virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

    /** Return max/min slider value changed delegate (only apply if SupportDynamicSliderMaxValue or SupportDynamicSliderMinValue are true) */
    FOnNumericEntryBoxDynamicSliderMinMaxValueChanged& GetOnNumericEntryBoxDynamicSliderMinValueChangedDelegate() { return OnNumericEntryBoxDynamicSliderMinValueChanged; }
    FOnNumericEntryBoxDynamicSliderMinMaxValueChanged& GetOnNumericEntryBoxDynamicSliderMaxValueChangedDelegate() { return OnNumericEntryBoxDynamicSliderMaxValueChanged; }
    
    /** Callback when the max/min spinner value are changed (only apply if SupportDynamicSliderMaxValue or SupportDynamicSliderMinValue are true) */
    template <typename TNumericType>
    void OnDynamicSliderMaxValueChanged(TNumericType NewMaxSliderValue, TWeakPtr<SWidget> InValueChangedSourceWidget, bool IsOriginator, bool UpdateOnlyIfHigher);

    template <typename TNumericType>
    void OnDynamicSliderMinValueChanged(TNumericType NewMinSliderValue, TWeakPtr<SWidget> InValueChangedSourceWidget, bool IsOriginator, bool UpdateOnlyIfLower);

    /**
    * Called to see if the value is enabled for editing
    *
    * @param WeakHandlePtr	Handle to the property that the new value is for
    */
    bool IsValueEnabled(TWeakPtr<IPropertyHandle> WeakHandlePtr) const;

    /** Utility function that will extract common Math related numeric metadata */
    template <typename TNumericType>
    static void ExtractNumericMetadata(TSharedRef<IPropertyHandle>& PropertyHandle, TOptional<TNumericType>& MinValue, TOptional<TNumericType>& MaxValue, TOptional<TNumericType>& SliderMinValue, TOptional<TNumericType>& SliderMaxValue, TNumericType& SliderExponent, TNumericType& Delta, int32 &ShiftMouseMovePixelPerDelta, bool& SupportDynamicSliderMaxValue, bool& SupportDynamicSliderMinValue);

protected:
    /**
     * Makes the header row for the customization
     *
     * @param StructPropertyHandle	Handle to the struct property
     * @param Row	The header row to add widgets to
     */
    virtual void MakeHeaderRow(TSharedRef<IPropertyHandle>& StructPropertyHandle, FDetailWidgetRow& Row);

    /**
     * Gets the sorted children for the struct
     *
     * @param StructPropertyHandle	The handle to the struct property
     * @param Children			The child array that should be populated in the order that children should be displayed
     */
    virtual void GetSortedChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, TArray<TSharedRef<IPropertyHandle>>& Children);

    /* Override this to specify if the type being customized is supported */
    virtual bool IsSupported(TSharedRef<IPropertyHandle>& PropertyHandle);

    /**
     * Constructs a widget for the property handle
     *
     * @param StructurePropertyHandle	handle of the struct property
     * @param PropertyHandle	Child handle of the struct property
     */
    virtual TSharedRef<SWidget> MakeChildWidget(
        TSharedRef<IPropertyHandle>& StructurePropertyHandle,
        TSharedRef<IPropertyHandle>& PropertyHandle);

    /**
     * Gets the value as a float for the provided property handle
     *
     * @param WeakHandlePtr	Handle to the property to get the value from
     * @return The value or unset if it could not be accessed
     */
    template<typename TNumericType>
    TOptional<TNumericType> OnGetValue(TWeakPtr<IPropertyHandle> WeakHandlePtr) const;

    /**
     * Called when the value is committed from the property editor
     *
     * @param NewValue		The new value of the property as a float
     * @param CommitType	How the value was committed (unused)
     * @param WeakHandlePtr	Handle to the property that the new value is for
     */
    template<typename TNumericType>
    void OnValueCommitted(TNumericType NewValue, ETextCommit::Type CommitType, TWeakPtr<IPropertyHandle> WeakHandlePtr);

    /**
     * Called when the value is changed in the property editor
     *
     * @param NewValue		The new value of the property as a float
     * @param WeakHandlePtr	Handle to the property that the new value is for
     */
    template<typename TNumericType>
    void OnValueChanged(TNumericType NewValue, TWeakPtr<IPropertyHandle> WeakHandlePtr);

    /**
     * Called to set the value of the property handle.
     *
     * @param NewValue		The new value of the property as a float
     * @param Flags         The flags to pass when setting the value on the property handle.
     * @param WeakHandlePtr	Handle to the property that the new value is for
     */
    template<typename TNumericType>
    void SetValue(TNumericType NewValue, EPropertyValueSetFlags::Type Flags, TWeakPtr<IPropertyHandle> WeakHandlePtr);

private:
    /** Gets the brush to use for the lock icon. */
    const FSlateBrush* GetPreserveScaleRatioImage() const;

    /** Gets the checked value of the preserve scale ratio option */
    ECheckBoxState IsPreserveScaleRatioChecked() const;

    /** Called when the user toggles preserve ratio. */
    void OnPreserveScaleRatioToggled(ECheckBoxState NewState, TWeakPtr<IPropertyHandle> PropertyHandle);

private:
    /** Called when a value starts to be changed by a slider */
    void OnBeginSliderMovement();

    /** Called when a value stops being changed by a slider */
    template<typename TNumericType>
    void OnEndSliderMovement(TNumericType NewValue);

    template<typename TNumericType>
    TSharedRef<SWidget> MakeNumericWidget(TSharedRef<IPropertyHandle>& StructurePropertyHandle, TSharedRef<IPropertyHandle>& PropertyHandle);

protected:
    FOnNumericEntryBoxDynamicSliderMinMaxValueChanged OnNumericEntryBoxDynamicSliderMinValueChanged;
    FOnNumericEntryBoxDynamicSliderMinMaxValueChanged OnNumericEntryBoxDynamicSliderMaxValueChanged;
    
    /** All the sorted children of the struct that should be displayed */
    TArray< TSharedRef<IPropertyHandle> > SortedChildHandles;

    /** All created numeric entry box widget for this customization */
    TArray<TWeakPtr<SWidget>> NumericEntryBoxWidgetList;

    /** True if a value is being changed by dragging a slider */
    bool bIsUsingSlider;

    /** True if the ratio is locked when scaling occurs (uniform scaling) */
    bool bPreserveScaleRatio;
};