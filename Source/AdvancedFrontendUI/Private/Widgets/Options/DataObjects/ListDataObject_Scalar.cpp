#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"

#include "Widgets/Options/OptionsDataInteractionHelper.h"

TRange<float> UListDataObject_Scalar::GetDisplayValueRange() const
{
	return this->DisplayValueRange;
}

void UListDataObject_Scalar::SetDisplayValueRange(const TRange<float> InDisplayValueRange)
{
	this->DisplayValueRange = InDisplayValueRange;
}

TRange<float> UListDataObject_Scalar::GetOutputValueRange() const
{
	return this->OutputValueRange;
}

void UListDataObject_Scalar::SetOutputValueRange(const TRange<float> InOutputValueRange)
{
	this->OutputValueRange = InOutputValueRange;
}

float UListDataObject_Scalar::GetSliderStepSize() const
{
	return this->SliderStepSize;
}

void UListDataObject_Scalar::SetSliderStepSize(float InSliderStepSize)
{
	this->SliderStepSize = InSliderStepSize;
}

ECommonNumericType UListDataObject_Scalar::GetDisplayNumericType() const
{
	return this->DisplayNumericType;
}

void UListDataObject_Scalar::SetDisplayNumericType(ECommonNumericType InDisplayNumericType)
{
	this->DisplayNumericType = InDisplayNumericType;
}

FCommonNumberFormattingOptions UListDataObject_Scalar::GetNumberFormattingOptions() const
{
	return this->NumberFormattingOptions;
}

void UListDataObject_Scalar::SetNumberFormattingOptions(const FCommonNumberFormattingOptions InNumberFormattingOptions)
{
	this->NumberFormattingOptions = InNumberFormattingOptions;
}

FCommonNumberFormattingOptions UListDataObject_Scalar::NoDecimal()
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = 0;
	
	return Options;
}

FCommonNumberFormattingOptions UListDataObject_Scalar::WithDecimal(int32 NumFracDigits)
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = NumFracDigits;
	
	return Options;
}

float UListDataObject_Scalar::GetCurrentValue() const
{
	if (DataDynamicGetter)
	{
		return FMath::GetMappedRangeValueClamped(this->OutputValueRange, this->DisplayValueRange, this->StringToFloat(DataDynamicGetter->GetValueAsString()));
	}
	
	return 0.f;
}

void UListDataObject_Scalar::SetCurrentValueFromSlider(float InNewValue)
{
	if (DataDynamicSetter)
	{
		const float ClampedValue = FMath::GetMappedRangeValueClamped(this->DisplayValueRange, this->OutputValueRange, InNewValue);
		DataDynamicSetter->SetValueFromString(LexToString(ClampedValue));
		
		NotifyListDataModified(this);
	}
}

float UListDataObject_Scalar::StringToFloat(const FString& InString) const
{
	float OutConvertedValue = 0.f;
	LexFromString(OutConvertedValue, *InString);
	
	return OutConvertedValue;
}

bool UListDataObject_Scalar::CanResetBackToDefaultValue() const
{
	if (this->HasDefaultValue() && DataDynamicGetter)
	{
		const float DefaultValue = this->StringToFloat(GetDefaultValueAsString());
		const float CurrentValue = this->StringToFloat(DataDynamicGetter->GetValueAsString());
		
		return !FMath::IsNearlyEqual(DefaultValue, CurrentValue, 0.01f);
	}
	
	return false;
}

bool UListDataObject_Scalar::TryResetBackToDefaultValue()
{
	if (this->CanResetBackToDefaultValue())
	{
		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(GetDefaultValueAsString());
			
			NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);
			
			return true;
		}
	}
	return false;
}
