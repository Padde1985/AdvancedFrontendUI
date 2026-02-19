#include "Widgets/Options/ListEntries/Widget_ListEntry_Scalar.h"
#include "AnalogSlider.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"

void UWidget_ListEntry_Scalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	this->AnalogSlider_SettingSlider->OnValueChanged.AddUniqueDynamic(this, &UWidget_ListEntry_Scalar::OnSliderValueChanged);
	this->AnalogSlider_SettingSlider->OnMouseCaptureBegin.AddUniqueDynamic(this, &UWidget_ListEntry_Scalar::OnSliderMouseCaptureBegin);
}

void UWidget_ListEntry_Scalar::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);
	
	this->CachedOwningScalarDataObject = CastChecked<UListDataObject_Scalar>(InOwningListDataObject);
	
	this->CommonNumeric_SettingValue->SetNumericType(this->CachedOwningScalarDataObject->GetDisplayNumericType());
	this->CommonNumeric_SettingValue->FormattingSpecification = this->CachedOwningScalarDataObject->GetNumberFormattingOptions();
	this->CommonNumeric_SettingValue->SetCurrentValue(this->CachedOwningScalarDataObject->GetCurrentValue());
	
	this->AnalogSlider_SettingSlider->SetMinValue(this->CachedOwningScalarDataObject->GetDisplayValueRange().GetLowerBoundValue());
	this->AnalogSlider_SettingSlider->SetMaxValue(this->CachedOwningScalarDataObject->GetDisplayValueRange().GetUpperBoundValue());
	this->AnalogSlider_SettingSlider->SetStepSize(this->CachedOwningScalarDataObject->GetSliderStepSize());
	this->AnalogSlider_SettingSlider->SetValue(this->CachedOwningScalarDataObject->GetCurrentValue());
}

void UWidget_ListEntry_Scalar::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (this->CachedOwningScalarDataObject)
	{
		this->CommonNumeric_SettingValue->SetCurrentValue(this->CachedOwningScalarDataObject->GetCurrentValue());
		this->AnalogSlider_SettingSlider->SetValue(this->CachedOwningScalarDataObject->GetCurrentValue());
	}
}

void UWidget_ListEntry_Scalar::OnSliderValueChanged(float Value)
{
	if (this->CachedOwningScalarDataObject)	this->CachedOwningScalarDataObject->SetCurrentValueFromSlider(Value);
}

void UWidget_ListEntry_Scalar::OnSliderMouseCaptureBegin()
{
	SelectThisEntryWidget();
}
