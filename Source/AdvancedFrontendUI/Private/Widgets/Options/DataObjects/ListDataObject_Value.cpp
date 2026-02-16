#include "Widgets/Options/DataObjects/ListDataObject_Value.h"

void UListDataObject_Value::SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGetter)
{
	this->DataDynamicGetter = InDynamicGetter;
}

void UListDataObject_Value::SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSetter)
{
	this->DataDynamicSetter = InDynamicSetter;
}

void UListDataObject_Value::SetDefaultValueFromString(const FString& InDefaultValue)
{
	this->DefaultStringValue = InDefaultValue;
}

bool UListDataObject_Value::HasDefaultValue() const
{
	return this->DefaultStringValue.IsSet();
}

FString UListDataObject_Value::GetDefaultValueAsString() const
{
	return this->DefaultStringValue.GetValue();
}
