#include "DetailCustomizations/NumericCustomization.h"

#include "UObject/UnrealType.h"
#include "Widgets/Text/STextBlock.h"
#include "Editor.h"
#include "Misc/ConfigCacheIni.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SCheckBox.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "DetailLayoutBuilder.h"
#include "Widgets/Input/SNumericEntryBox.h"

#define LOCTEXT_NAMESPACE "FNumericCustomization"

IMPLEMENT_MAKE_INSTANCE(FNumericCustomization)

void FNumericCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
    GetSortedChildren(StructPropertyHandle, SortedChildHandles);
    MakeHeaderRow(StructPropertyHandle, HeaderRow);
}

void FNumericCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
    for (auto& ChildHandle : SortedChildHandles)
        StructBuilder.AddProperty(ChildHandle);
}

void FNumericCustomization::MakeHeaderRow(TSharedRef<IPropertyHandle>& StructPropertyHandle, FDetailWidgetRow& Row)
{
    const bool bDisplayResetToDefault = false;

    const FText DisplayNameOverride = FText::GetEmpty();
    const FText DisplayToolTipOverride = FText::GetEmpty();

    TWeakPtr<IPropertyHandle> StructWeakHandlePtr = StructPropertyHandle;

    TSharedPtr<SHorizontalBox> HorizontalBox;

    Row
        .NameContent()
        [
            StructPropertyHandle->CreatePropertyNameWidget(DisplayNameOverride, DisplayToolTipOverride, bDisplayResetToDefault)
        ]
        .ValueContent()
        .MinDesiredWidth(125.0f * SortedChildHandles.Num())
        .MaxDesiredWidth(125.0f * SortedChildHandles.Num())
        [
            SAssignNew(HorizontalBox, SHorizontalBox)
            .IsEnabled(this, &FNumericCustomization::IsValueEnabled, StructWeakHandlePtr)
        ];

    for (int32 ChildIndex = 0; ChildIndex < SortedChildHandles.Num(); ++ChildIndex)
    {
        TSharedRef<IPropertyHandle> ChildHandle = SortedChildHandles[ChildIndex];

        const bool bLastChild = SortedChildHandles.Num() - 1 == ChildIndex;
        // Make a widget for each property.  The vector component properties  will be displayed in the header

        TSharedRef<SWidget> NumericEntryBox = MakeChildWidget(StructPropertyHandle, ChildHandle);
        NumericEntryBoxWidgetList.Add(NumericEntryBox);

        HorizontalBox->AddSlot()
            .Padding(FMargin(0.0f, 2.0f, bLastChild ? 0.0f : 3.0f, 2.0f))
            [
                NumericEntryBox
            ];
    }

    if (StructPropertyHandle->GetProperty()->HasMetaData("AllowPreserveRatio"))
    {
        if (!GConfig->GetBool(TEXT("SelectionDetails"), *(StructPropertyHandle->GetProperty()->GetName() + TEXT("_PreserveScaleRatio")), bPreserveScaleRatio, GEditorPerProjectIni))
            bPreserveScaleRatio = true;

        HorizontalBox->AddSlot()
            .AutoWidth()
            .MaxWidth(18.0f)
            [
                // Add a checkbox to toggle between preserving the ratio of x,y,z components of scale when a value is entered
                SNew(SCheckBox)
                .IsChecked(this, &FNumericCustomization::IsPreserveScaleRatioChecked)
                .OnCheckStateChanged(this, &FNumericCustomization::OnPreserveScaleRatioToggled, StructWeakHandlePtr)
                .Style(FEditorStyle::Get(), "TransparentCheckBox")
                .ToolTipText(LOCTEXT("PreserveScaleToolTip", "When locked, scales uniformly based on the current xyz scale values so the object maintains its shape in each direction when scaled"))
                [
                    SNew(SImage)
                    .Image(this, &FNumericCustomization::GetPreserveScaleRatioImage)
                    .ColorAndOpacity(FSlateColor::UseForeground())
                ]
            ];
    }
}

bool FNumericCustomization::IsSupported(TSharedRef<IPropertyHandle>& PropertyHandle)
{
    const auto* PropertyClass = PropertyHandle->GetPropertyClass();

    if (PropertyClass == FFloatProperty::StaticClass()
        || PropertyClass == FIntProperty::StaticClass()
        || PropertyClass == FByteProperty::StaticClass())
        return true;

    if (PropertyClass == FEnumProperty::StaticClass())
    {
        const auto* EnumPropertyClass = static_cast<const FEnumProperty*>(PropertyHandle->GetProperty());
        const auto* Enum = EnumPropertyClass->GetUnderlyingProperty();
        const auto* EnumClass = Enum->GetClass();
        if (EnumClass == FByteProperty::StaticClass()
            || EnumClass == FIntProperty::StaticClass())
            return true;
    }

    return false;
}

TSharedRef<SWidget> FNumericCustomization::MakeChildWidget(TSharedRef<IPropertyHandle>& StructurePropertyHandle, TSharedRef<IPropertyHandle>& PropertyHandle)
{
    if (!IsSupported(PropertyHandle))
    {
        check(0); // Unsupported class
        return SNullWidget::NullWidget;
    }

    const auto* PropertyClass = PropertyHandle->GetPropertyClass();

    if (PropertyClass == FFloatProperty::StaticClass())
        return MakeNumericWidget<float>(StructurePropertyHandle, PropertyHandle);

    if (PropertyClass == FIntProperty::StaticClass())
        return MakeNumericWidget<int32>(StructurePropertyHandle, PropertyHandle);

    if (PropertyClass == FByteProperty::StaticClass())
        return MakeNumericWidget<uint8>(StructurePropertyHandle, PropertyHandle);

    if (PropertyClass == FEnumProperty::StaticClass())
    {
        const auto* EnumPropertyClass = static_cast<const FEnumProperty*>(PropertyHandle->GetProperty());
        const auto* Enum = EnumPropertyClass->GetUnderlyingProperty();
        const auto* EnumClass = Enum->GetClass();
        if (EnumClass == FByteProperty::StaticClass())
            return MakeNumericWidget<uint8>(StructurePropertyHandle, PropertyHandle);
        else if (EnumClass == FIntProperty::StaticClass())
            return MakeNumericWidget<int32>(StructurePropertyHandle, PropertyHandle);
    }

    return SNullWidget::NullWidget;
}

void FNumericCustomization::GetSortedChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, TArray<TSharedRef<IPropertyHandle>>& Children)
{
    Children.Empty();

    uint32 NumChildren;
    StructPropertyHandle->GetNumChildren(NumChildren);

    for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
        Children.Add(StructPropertyHandle->GetChildHandle(ChildIndex).ToSharedRef());
}

template <typename TNumericType>
void FNumericCustomization::ExtractNumericMetadata(TSharedRef<IPropertyHandle>& PropertyHandle, TOptional<TNumericType>& MinValue, TOptional<TNumericType>& MaxValue, TOptional<TNumericType>& SliderMinValue, TOptional<TNumericType>& SliderMaxValue, TNumericType& SliderExponent, TNumericType& Delta, int32 &ShiftMouseMovePixelPerDelta, bool& SupportDynamicSliderMaxValue, bool& SupportDynamicSliderMinValue)
{
    auto* Property = PropertyHandle->GetProperty();
    
    const FString& MetaUIMinString = Property->GetMetaData(TEXT("UIMin"));
    const FString& MetaUIMaxString = Property->GetMetaData(TEXT("UIMax"));
    const FString& SliderExponentString = Property->GetMetaData(TEXT("SliderExponent"));
    const FString& DeltaString = Property->GetMetaData(TEXT("Delta"));
    const FString& ShiftMouseMovePixelPerDeltaString = Property->GetMetaData(TEXT("ShiftMouseMovePixelPerDelta"));
    const FString& SupportDynamicSliderMaxValueString = Property->GetMetaData(TEXT("SupportDynamicSliderMaxValue"));
    const FString& SupportDynamicSliderMinValueString = Property->GetMetaData(TEXT("SupportDynamicSliderMinValue"));
    const FString& ClampMinString = Property->GetMetaData(TEXT("ClampMin"));
    const FString& ClampMaxString = Property->GetMetaData(TEXT("ClampMax"));

    // If no UIMin/Max was specified then use the clamp string
    const FString& UIMinString = MetaUIMinString.Len() ? MetaUIMinString : ClampMinString;
    const FString& UIMaxString = MetaUIMaxString.Len() ? MetaUIMaxString : ClampMaxString;

    TNumericType ClampMin = TNumericLimits<TNumericType>::Lowest();
    TNumericType ClampMax = TNumericLimits<TNumericType>::Max();

    if (!ClampMinString.IsEmpty())
        TTypeFromString<TNumericType>::FromString(ClampMin, *ClampMinString);

    if (!ClampMaxString.IsEmpty())
        TTypeFromString<TNumericType>::FromString(ClampMax, *ClampMaxString);

    TNumericType UIMin = TNumericLimits<TNumericType>::Lowest();
    TNumericType UIMax = TNumericLimits<TNumericType>::Max();
    TTypeFromString<TNumericType>::FromString(UIMin, *UIMinString);
    TTypeFromString<TNumericType>::FromString(UIMax, *UIMaxString);

    SliderExponent = TNumericType(1);

    if (SliderExponentString.Len())
        TTypeFromString<TNumericType>::FromString(SliderExponent, *SliderExponentString);

    Delta = TNumericType(0);

    if (DeltaString.Len())
        TTypeFromString<TNumericType>::FromString(Delta, *DeltaString);

    ShiftMouseMovePixelPerDelta = 1;
    if (ShiftMouseMovePixelPerDeltaString.Len())
    {
        TTypeFromString<int32>::FromString(ShiftMouseMovePixelPerDelta, *ShiftMouseMovePixelPerDeltaString);
        //The value should be greater or equal to 1
        // 1 is neutral since it is a multiplier of the mouse drag pixel
        if (ShiftMouseMovePixelPerDelta < 1)
            ShiftMouseMovePixelPerDelta = 1;
    }

    if (ClampMin >= ClampMax && (ClampMinString.Len() || ClampMaxString.Len()))
    {
        //UE_LOG(LogPropertyNode, Warning, TEXT("Clamp Min (%s) >= Clamp Max (%s) for Ranged Numeric"), *ClampMinString, *ClampMaxString);
    }

    const TNumericType ActualUIMin = FMath::Max(UIMin, ClampMin);
    const TNumericType ActualUIMax = FMath::Min(UIMax, ClampMax);

    MinValue = ClampMinString.Len() ? ClampMin : TOptional<TNumericType>();
    MaxValue = ClampMaxString.Len() ? ClampMax : TOptional<TNumericType>();
    SliderMinValue = (UIMinString.Len()) ? ActualUIMin : TOptional<TNumericType>();
    SliderMaxValue = (UIMaxString.Len()) ? ActualUIMax : TOptional<TNumericType>();

    if (ActualUIMin >= ActualUIMax && (MetaUIMinString.Len() || MetaUIMaxString.Len()))
    {
        //UE_LOG(LogPropertyNode, Warning, TEXT("UI Min (%s) >= UI Max (%s) for Ranged Numeric"), *UIMinString, *UIMaxString);
    }

    SupportDynamicSliderMaxValue = SupportDynamicSliderMaxValueString.Len() > 0 && SupportDynamicSliderMaxValueString.ToBool();
    SupportDynamicSliderMinValue = SupportDynamicSliderMinValueString.Len() > 0 && SupportDynamicSliderMinValueString.ToBool();
}

template<typename TNumericType>
TSharedRef<SWidget> FNumericCustomization::MakeNumericWidget(TSharedRef<IPropertyHandle>& StructurePropertyHandle, TSharedRef<IPropertyHandle>& PropertyHandle)
{
    TOptional<TNumericType> MinValue, MaxValue, SliderMinValue, SliderMaxValue;
    TNumericType SliderExponent, Delta;
    int32 ShiftMouseMovePixelPerDelta = 1;
    bool SupportDynamicSliderMaxValue = false;
    bool SupportDynamicSliderMinValue = false;

    ExtractNumericMetadata(StructurePropertyHandle, MinValue, MaxValue, SliderMinValue, SliderMaxValue, SliderExponent, Delta, ShiftMouseMovePixelPerDelta, SupportDynamicSliderMaxValue, SupportDynamicSliderMinValue);

    TWeakPtr<IPropertyHandle> WeakHandlePtr = PropertyHandle;

    return 
        SNew(SNumericEntryBox<TNumericType>)
        .IsEnabled(this, &FNumericCustomization::IsValueEnabled, WeakHandlePtr)
        .EditableTextBoxStyle(&FCoreStyle::Get().GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox"))
        .Value(this, &FNumericCustomization::OnGetValue, WeakHandlePtr)
        .Font(IDetailLayoutBuilder::GetDetailFont())
        .UndeterminedString(NSLOCTEXT("PropertyEditor", "MultipleValues", "Multiple Values"))
        .OnValueCommitted(this, &FNumericCustomization::OnValueCommitted<TNumericType>, WeakHandlePtr)
        .OnValueChanged(this, &FNumericCustomization::OnValueChanged<TNumericType>, WeakHandlePtr)
        .OnBeginSliderMovement(this, &FNumericCustomization::OnBeginSliderMovement)
        .OnEndSliderMovement(this, &FNumericCustomization::OnEndSliderMovement<TNumericType>)
        .LabelVAlign(VAlign_Center)
        // Only allow spin on handles with one object.  Otherwise it is not clear what value to spin
        .AllowSpin(PropertyHandle->GetNumOuterObjects() < 2)
        .ShiftMouseMovePixelPerDelta(ShiftMouseMovePixelPerDelta)
        .SupportDynamicSliderMaxValue(SupportDynamicSliderMaxValue)
        .SupportDynamicSliderMinValue(SupportDynamicSliderMinValue)
        .OnDynamicSliderMaxValueChanged(this, &FNumericCustomization::OnDynamicSliderMaxValueChanged<TNumericType>)
        .OnDynamicSliderMinValueChanged(this, &FNumericCustomization::OnDynamicSliderMinValueChanged<TNumericType>)
        .MinValue(MinValue)
        .MaxValue(MaxValue)
        .MinSliderValue(SliderMinValue)
        .MaxSliderValue(SliderMaxValue)
        .SliderExponent(SliderExponent)
        .Delta(Delta)
        .Label()
        [
            SNew(STextBlock)
            .Font(IDetailLayoutBuilder::GetDetailFont())
            .Text(PropertyHandle->GetPropertyDisplayName())
        ];
}

template <typename TNumericType>
void FNumericCustomization::OnDynamicSliderMinValueChanged(TNumericType NewMinSliderValue, TWeakPtr<SWidget> InValueChangedSourceWidget, bool bIsOriginator, bool bUpdateOnlyIfLower)
{
    for (TWeakPtr<SWidget>& Widget : NumericEntryBoxWidgetList)
    {
        TSharedPtr<SNumericEntryBox<TNumericType>> NumericBox = StaticCastSharedPtr<SNumericEntryBox<TNumericType>>(Widget.Pin());

        if (NumericBox.IsValid())
        {
            TSharedPtr<SSpinBox<TNumericType>> SpinBox = StaticCastSharedPtr<SSpinBox<TNumericType>>(NumericBox->GetSpinBox());

            if (SpinBox.IsValid())
            {
                if (SpinBox != InValueChangedSourceWidget)
                {
                    if ((NewMinSliderValue < SpinBox->GetMinSliderValue() && bUpdateOnlyIfLower) || !bUpdateOnlyIfLower)
                    {
                        // Make sure the min slider value is not a getter otherwise we will break the link!
                        verifySlow(!SpinBox->IsMinSliderValueBound());
                        SpinBox->SetMinSliderValue(NewMinSliderValue);
                    }
                }
            }
        }
    }

    if (bIsOriginator)
        OnNumericEntryBoxDynamicSliderMinValueChanged.Broadcast((float)NewMinSliderValue, InValueChangedSourceWidget, false, bUpdateOnlyIfLower);
}

template <typename TNumericType>
void FNumericCustomization::OnDynamicSliderMaxValueChanged(TNumericType NewMaxSliderValue, TWeakPtr<SWidget> InValueChangedSourceWidget, bool bIsOriginator, bool bUpdateOnlyIfHigher)
{
    for (TWeakPtr<SWidget>& Widget : NumericEntryBoxWidgetList)
    {
        TSharedPtr<SNumericEntryBox<TNumericType>> NumericBox = StaticCastSharedPtr<SNumericEntryBox<TNumericType>>(Widget.Pin());

        if (NumericBox.IsValid())
        {
            TSharedPtr<SSpinBox<TNumericType>> SpinBox = StaticCastSharedPtr<SSpinBox<TNumericType>>(NumericBox->GetSpinBox());

            if (SpinBox.IsValid())
            {
                if (SpinBox != InValueChangedSourceWidget)
                {
                    if ((NewMaxSliderValue > SpinBox->GetMaxSliderValue() && bUpdateOnlyIfHigher) || !bUpdateOnlyIfHigher)
                    {
                        // Make sure the max slider value is not a getter otherwise we will break the link!
                        verifySlow(!SpinBox->IsMaxSliderValueBound());
                        SpinBox->SetMaxSliderValue(NewMaxSliderValue);
                    }
                }
            }
        }
    }

    if (bIsOriginator)
        OnNumericEntryBoxDynamicSliderMaxValueChanged.Broadcast((float)NewMaxSliderValue, InValueChangedSourceWidget, false, bUpdateOnlyIfHigher);
}

template<typename TNumericType>
TOptional<TNumericType> FNumericCustomization::OnGetValue(TWeakPtr<IPropertyHandle> WeakHandlePtr) const
{
    TNumericType NumericVal = 0;
    if (WeakHandlePtr.Pin()->GetValue(NumericVal) == FPropertyAccess::Success)
        return TOptional<TNumericType>(NumericVal);

    // Value couldn't be accessed.  Return an unset value
    return TOptional<TNumericType>();
}

template<typename TNumericType>
void FNumericCustomization::OnValueCommitted(TNumericType NewValue, ETextCommit::Type CommitType, TWeakPtr<IPropertyHandle> WeakHandlePtr)
{
    EPropertyValueSetFlags::Type Flags = EPropertyValueSetFlags::DefaultFlags;
    SetValue(NewValue, Flags, WeakHandlePtr);
}

template<typename TNumericType>
void FNumericCustomization::OnValueChanged(TNumericType NewValue, TWeakPtr<IPropertyHandle> WeakHandlePtr)
{
    if (bIsUsingSlider)
    {
        EPropertyValueSetFlags::Type Flags = EPropertyValueSetFlags::InteractiveChange;
        SetValue(NewValue, Flags, WeakHandlePtr);
    }
}

template<typename TNumericType>
void FNumericCustomization::SetValue(TNumericType NewValue, EPropertyValueSetFlags::Type Flags, TWeakPtr<IPropertyHandle> WeakHandlePtr)
{
    if (bPreserveScaleRatio)
    {
        // Get the value for each object for the modified component
        TArray<FString> OldValues;
        if (WeakHandlePtr.Pin()->GetPerObjectValues(OldValues) == FPropertyAccess::Success)
        {
            // Loop through each object and scale based on the new ratio for each object individually
            for (int32 OutputIndex = 0; OutputIndex < OldValues.Num(); ++OutputIndex)
            {
                TNumericType OldValue;
                TTypeFromString<TNumericType>::FromString(OldValue, *OldValues[OutputIndex]);

                // Account for the previous scale being zero.  Just set to the new value in that case?
                TNumericType Ratio = OldValue == 0 ? NewValue : NewValue / OldValue;
                if (Ratio == 0)
                    Ratio = NewValue;

                // Loop through all the child handles (each component of the math struct, like X, Y, Z...etc)
                for (int32 ChildIndex = 0; ChildIndex < SortedChildHandles.Num(); ++ChildIndex)
                {
                    // Ignore scaling our selves.
                    TSharedRef<IPropertyHandle> ChildHandle = SortedChildHandles[ChildIndex];
                    if (ChildHandle != WeakHandlePtr.Pin())
                    {
                        // Get the value for each object.
                        TArray<FString> ObjectChildValues;
                        if (ChildHandle->GetPerObjectValues(ObjectChildValues) == FPropertyAccess::Success)
                        {
                            // Individually scale each object's components by the same ratio.
                            for (int32 ChildOutputIndex = 0; ChildOutputIndex < ObjectChildValues.Num(); ++ChildOutputIndex)
                            {
                                TNumericType ChildOldValue;
                                TTypeFromString<TNumericType>::FromString(ChildOldValue, *ObjectChildValues[ChildOutputIndex]);

                                TNumericType ChildNewValue = ChildOldValue * Ratio;
                                ObjectChildValues[ChildOutputIndex] = TTypeToString<TNumericType>::ToSanitizedString(ChildNewValue);
                            }

                            ChildHandle->SetPerObjectValues(ObjectChildValues);
                        }
                    }
                }
            }
        }
    }

    WeakHandlePtr.Pin()->SetValue(NewValue, Flags);
}

const FSlateBrush* FNumericCustomization::GetPreserveScaleRatioImage() const
{
    return bPreserveScaleRatio ? FEditorStyle::GetBrush(TEXT("GenericLock")) : FEditorStyle::GetBrush(TEXT("GenericUnlock"));
}

ECheckBoxState FNumericCustomization::IsPreserveScaleRatioChecked() const
{
    return bPreserveScaleRatio ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void FNumericCustomization::OnPreserveScaleRatioToggled(ECheckBoxState NewState, TWeakPtr<IPropertyHandle> PropertyHandle)
{
    bPreserveScaleRatio = (NewState == ECheckBoxState::Checked) ? true : false;

    if (PropertyHandle.IsValid() && PropertyHandle.Pin()->GetProperty())
    {
        FString SettingKey = (PropertyHandle.Pin()->GetProperty()->GetName() + TEXT("_PreserveScaleRatio"));
        GConfig->SetBool(TEXT("SelectionDetails"), *SettingKey, bPreserveScaleRatio, GEditorPerProjectIni);
    }
}

bool FNumericCustomization::IsValueEnabled(TWeakPtr<IPropertyHandle> WeakHandlePtr) const
{
    if (WeakHandlePtr.IsValid())
        return !WeakHandlePtr.Pin()->IsEditConst();

    return false;
}

void FNumericCustomization::OnBeginSliderMovement()
{
    bIsUsingSlider = true;

    GEditor->BeginTransaction(LOCTEXT("SetNumericProperty", "Set Numeric Property"));
}

template<typename TNumericType>
void FNumericCustomization::OnEndSliderMovement(TNumericType NewValue)
{
    bIsUsingSlider = false;

    GEditor->EndTransaction();
}

#undef LOCTEXT_NAMESPACE
