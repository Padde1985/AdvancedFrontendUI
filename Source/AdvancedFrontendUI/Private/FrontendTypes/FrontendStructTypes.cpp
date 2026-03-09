#include "FrontendTypes/FrontendStructTypes.h"

// store the given callback function to be called later
void FOptionsDataEditConditionDescriptor::SetEditConditionFunc(TFunction<bool()> InEditConditionFunc)
{
	this->EditConditionFunc = InEditConditionFunc;
}

// check if the callback function was set and/or is valid
bool FOptionsDataEditConditionDescriptor::IsValid() const
{
	return this->EditConditionFunc != nullptr;
}

// call the stored callback function to define if an option is editable or not
bool FOptionsDataEditConditionDescriptor::IsEditConditionMet() const
{
	if (this->IsValid())
	{
		return this->EditConditionFunc();
	}
	
	return true;
}

// return the reason String if an option is disabled
FString FOptionsDataEditConditionDescriptor::GetDisabledRichReason() const
{
	return this->DisabledRichReason;
}

// set the String that's being displayed when hovering over or clicking on a disabled list entry in the options category
void FOptionsDataEditConditionDescriptor::SetDisabledRichReason(const FString& InDisabledRichReason)
{
	this->DisabledRichReason = InDisabledRichReason;
}

// check if an entry has a forced value (screen resolution in specific window modes, etc.)
bool FOptionsDataEditConditionDescriptor::HasForcedStringValue() const
{
	return this->DisabledForcedStringValue.IsSet();
}

// get the forced value
FString FOptionsDataEditConditionDescriptor::GetDisabledForcedStringValue() const
{
	return this->DisabledForcedStringValue.GetValue();
}

// set the forced value
void FOptionsDataEditConditionDescriptor::SetDisabledForcedStringValue(const FString& InForcedValue)
{
	this->DisabledForcedStringValue = InForcedValue;
}