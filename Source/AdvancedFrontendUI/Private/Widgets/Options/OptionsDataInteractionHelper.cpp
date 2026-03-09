#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"

// store function path and user settings
FOptionsDataInteractionHelper::FOptionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath) : CachedDynamicFunctionPath(InSetterOrGetterFuncPath)
{
	this->CachedWeakGameUserSettings = UFrontendGameUserSettings::Get();
}

// return the current property value (slider, checkbox, etc) as a string
FString FOptionsDataInteractionHelper::GetValueAsString() const
{
	FString OutStringValue;
	PropertyPathHelpers::GetPropertyValueAsString(this->CachedWeakGameUserSettings.Get(), this->CachedDynamicFunctionPath, OutStringValue);
	
	return OutStringValue;
}

// set the current property value from a given string
void FOptionsDataInteractionHelper::SetValueFromString(const FString& InStringValue)
{
	PropertyPathHelpers::SetPropertyValueFromString(this->CachedWeakGameUserSettings.Get(), this->CachedDynamicFunctionPath, InStringValue);
}