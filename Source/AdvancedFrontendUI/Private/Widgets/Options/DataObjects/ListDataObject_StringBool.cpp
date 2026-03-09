#include "Widgets/Options/DataObjects/ListDataObject_StringBool.h"

// override the default True string with the given value (e.g. show "activated" instead of "True")
void UListDataObject_StringBool::OverrideTrueDisplayText(const FText& InNewTrueDisplayText)
{
	if (!AvailableOptionsStringArray.Contains(this->TrueString)) AddDynamicOption(this->TrueString, InNewTrueDisplayText);
}

// override the default False string with the given value (e.g. show "deactivated" instead of "False")
void UListDataObject_StringBool::OverrideFalseDisplayText(const FText& InNewFalseDisplayText)
{
	if (!AvailableOptionsStringArray.Contains(this->FalseString)) AddDynamicOption(this->FalseString, InNewFalseDisplayText);
}

// set true as default value (does not mean it has to be the active one)
void UListDataObject_StringBool::SetTrueAsDefaultValue()
{
	SetDefaultValueFromString(this->TrueString);
}

// set false as default value (does not mean it has to be the active one)
void UListDataObject_StringBool::SetFalseAsDefaultValue()
{
	SetDefaultValueFromString(this->FalseString);
}

// callback when object is initialized
void UListDataObject_StringBool::OnDataObjectInitialized()
{
	this->TryInitBoolValues();
	
	Super::OnDataObjectInitialized();
}

// initialize the default values for True and false
void UListDataObject_StringBool::TryInitBoolValues()
{
	if (!AvailableOptionsStringArray.Contains(this->TrueString)) AddDynamicOption(this->TrueString, FText::FromString(TEXT("ON")));
	if (!AvailableOptionsStringArray.Contains(this->FalseString)) AddDynamicOption(this->FalseString, FText::FromString(TEXT("OFF")));
}