#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"

// getter for the external value range (e.g. % instead of the internal value)
TRange<float> UListDataObject_Scalar::GetDisplayValueRange() const
{
	return this->DisplayValueRange;
}

// setter for the display range interval
void UListDataObject_Scalar::SetDisplayValueRange(const TRange<float> InDisplayValueRange)
{
	this->DisplayValueRange = InDisplayValueRange;
}

// getter for the internal internal
TRange<float> UListDataObject_Scalar::GetOutputValueRange() const
{
	return this->OutputValueRange;
}

// setter for the internal interval
void UListDataObject_Scalar::SetOutputValueRange(const TRange<float> InOutputValueRange)
{
	this->OutputValueRange = InOutputValueRange;
}

// getter for the step size
float UListDataObject_Scalar::GetSliderStepSize() const
{
	return this->SliderStepSize;
}

// define the step size (thresholds or seemless transition) 
void UListDataObject_Scalar::SetSliderStepSize(float InSliderStepSize)
{
	this->SliderStepSize = InSliderStepSize;
}

// getter for the numerical type (% or float or or or)
ECommonNumericType UListDataObject_Scalar::GetDisplayNumericType() const
{
	return this->DisplayNumericType;
}

// setter for the numeric type
void UListDataObject_Scalar::SetDisplayNumericType(ECommonNumericType InDisplayNumericType)
{
	this->DisplayNumericType = InDisplayNumericType;
}

// getter for the format options (decimals, signed/unsigned, etc)
FCommonNumberFormattingOptions UListDataObject_Scalar::GetNumberFormattingOptions() const
{
	return this->NumberFormattingOptions;
}

// setter for the number format
void UListDataObject_Scalar::SetNumberFormattingOptions(const FCommonNumberFormattingOptions InNumberFormattingOptions)
{
	this->NumberFormattingOptions = InNumberFormattingOptions;
}

// set the decimals to 0
FCommonNumberFormattingOptions UListDataObject_Scalar::NoDecimal()
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = 0;
	
	return Options;
}

// set the decimals to the given number
FCommonNumberFormattingOptions UListDataObject_Scalar::WithDecimal(int32 NumFracDigits)
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = NumFracDigits;
	
	return Options;
}

// getter for the current slider value, converts from internal to external value
float UListDataObject_Scalar::GetCurrentValue() const
{
	if (DataDynamicGetter)
	{
		return FMath::GetMappedRangeValueClamped(this->OutputValueRange, this->DisplayValueRange, this->StringToFloat(DataDynamicGetter->GetValueAsString()));
	}
	
	return 0.f;
}

// sets the internal value from the given external value and broadcasts the delegate
void UListDataObject_Scalar::SetCurrentValueFromSlider(float InNewValue)
{
	if (DataDynamicSetter)
	{
		const float ClampedValue = FMath::GetMappedRangeValueClamped(this->DisplayValueRange, this->OutputValueRange, InNewValue);
		DataDynamicSetter->SetValueFromString(LexToString(ClampedValue));
		
		NotifyListDataModified(this);
	}
}

// converts a string to a float using the engine conversion function
float UListDataObject_Scalar::StringToFloat(const FString& InString) const
{
	float OutConvertedValue = 0.f;
	LexFromString(OutConvertedValue, *InString);
	
	return OutConvertedValue;
}

// check if the slider can be reset back to default
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

// reset the slider to the default value
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

// callback if dependent data has changed (e. g. overall graphics quality)
void UListDataObject_Scalar::OnEditDependecyDataModified(UListDataObject_Base* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason)
{
	NotifyListDataModified(this, EOptionsListDataModifyReason::DependencyModified);
	
	Super::OnEditDependecyDataModified(ModifiedDependencyData, ModifyReason);
}