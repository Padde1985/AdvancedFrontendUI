#include "FrontendTypes/FrontendStructTypes.h"

void FOptionsDataEditConditionDescriptor::SetEditConditionFunc(TFunction<bool()> InEditConditionFunc)
{
	this->EditConditionFunc = InEditConditionFunc;
}

bool FOptionsDataEditConditionDescriptor::IsValid() const
{
	return this->EditConditionFunc != nullptr;
}

bool FOptionsDataEditConditionDescriptor::IsEditConditionMet() const
{
	if (this->IsValid())
	{
		return this->EditConditionFunc();
	}
	
	return true;
}

FString FOptionsDataEditConditionDescriptor::GetDisabledRichReason() const
{
	return this->DisabledRichReason;
}

void FOptionsDataEditConditionDescriptor::SetDisabledRichReason(const FString& InDisabledRichReason)
{
	this->DisabledRichReason = InDisabledRichReason;
}

bool FOptionsDataEditConditionDescriptor::HasForcedStringValue() const
{
	return this->DisabledForcedStringValue.IsSet();
}

FString FOptionsDataEditConditionDescriptor::GetDisabledForcedStringValue() const
{
	return this->DisabledForcedStringValue.GetValue();
}

void FOptionsDataEditConditionDescriptor::SetDisabledForcedStringValue(const FString& InForcedValue)
{
	this->DisabledForcedStringValue = InForcedValue;
}
