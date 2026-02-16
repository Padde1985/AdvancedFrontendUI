#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"

FOptionsDataInteractionHelper::FOptionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath) : CachedDynamicFunctionPath(InSetterOrGetterFuncPath)
{
	this->CachedWeakGameUserSettings = UFrontendGameUserSettings::Get();
}

FString FOptionsDataInteractionHelper::GetValueAsString() const
{
	FString OutStringValue;
	PropertyPathHelpers::GetPropertyValueAsString(this->CachedWeakGameUserSettings.Get(), this->CachedDynamicFunctionPath, OutStringValue);
	
	return OutStringValue;
}

void FOptionsDataInteractionHelper::SetValueFromString(const FString& InStringValue)
{
	PropertyPathHelpers::SetPropertyValueFromString(this->CachedWeakGameUserSettings.Get(), this->CachedDynamicFunctionPath, InStringValue);
}
