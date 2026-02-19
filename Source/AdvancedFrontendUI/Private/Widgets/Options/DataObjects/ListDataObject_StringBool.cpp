#include "Widgets/Options/DataObjects/ListDataObject_StringBool.h"

void UListDataObject_StringBool::OverrideTrueDisplayText(const FText& InNewTrueDisplayText)
{
	if (!AvailableOptionsStringArray.Contains(this->TrueString)) AddDynamicOption(this->TrueString, InNewTrueDisplayText);
}

void UListDataObject_StringBool::OverrideFalseDisplayText(const FText& InNewFalseDisplayText)
{
	if (!AvailableOptionsStringArray.Contains(this->FalseString)) AddDynamicOption(this->FalseString, InNewFalseDisplayText);
}

void UListDataObject_StringBool::SetTrueAsDefaultValue()
{
	SetDefaultValueFromString(this->TrueString);
}

void UListDataObject_StringBool::SetFalseAsDefaultValue()
{
	SetDefaultValueFromString(this->FalseString);
}

void UListDataObject_StringBool::OnDataObjectInitialized()
{
	this->TryInitBoolValues();
	
	Super::OnDataObjectInitialized();
}

void UListDataObject_StringBool::TryInitBoolValues()
{
	if (!AvailableOptionsStringArray.Contains(this->TrueString)) AddDynamicOption(this->TrueString, FText::FromString(TEXT("ON")));
	if (!AvailableOptionsStringArray.Contains(this->FalseString)) AddDynamicOption(this->FalseString, FText::FromString(TEXT("OFF")));
}
