#include "Widgets/Options/DataObjects/ListDataObject_Value.h"

// set the dynamic getter
void UListDataObject_Value::SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGetter)
{
	this->DataDynamicGetter = InDynamicGetter;
}

// set the dynamic setter
void UListDataObject_Value::SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSetter)
{
	this->DataDynamicSetter = InDynamicSetter;
}

// set the default value using the specified string
void UListDataObject_Value::SetDefaultValueFromString(const FString& InDefaultValue)
{
	this->DefaultStringValue = InDefaultValue;
}

// check if a default value for this option was set
bool UListDataObject_Value::HasDefaultValue() const
{
	return this->DefaultStringValue.IsSet();
}

// return the default value as a string object
FString UListDataObject_Value::GetDefaultValueAsString() const
{
	return this->DefaultStringValue.GetValue();
}
