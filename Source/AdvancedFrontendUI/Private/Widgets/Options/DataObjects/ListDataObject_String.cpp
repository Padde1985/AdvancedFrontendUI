#include "Widgets/Options/DataObjects/ListDataObject_String.h"

#include "FrontendDebugHelper.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"

void UListDataObject_String::AddDynamicOption(const FString& InStringValue, const FText& InDisplayText)
{
	this->AvailableOptionsStringArray.Add(InStringValue);
	this->AvailableOptionsTextArray.Add(InDisplayText);
}

const TArray<FText>& UListDataObject_String::GetAvailableOptionsTextArray() const
{
	return this->AvailableOptionsTextArray;
}

FText UListDataObject_String::GetCurrentDisplayText() const
{
	return this->CurrentDisplayText;
}

void UListDataObject_String::AdvanceToNextOption()
{
	if (this->AvailableOptionsStringArray.IsEmpty() || this->AvailableOptionsTextArray.IsEmpty()) return;
	
	const int32 CurrentDisplayIndex = this->AvailableOptionsStringArray.IndexOfByKey(this->CurrentStringValue);
	const int32 NextIndexToDisplay = CurrentDisplayIndex + 1;
	
	if (this->AvailableOptionsStringArray.IsValidIndex(NextIndexToDisplay))
	{
		this->CurrentStringValue = this->AvailableOptionsStringArray[NextIndexToDisplay];
	}
	else
	{
		this->CurrentStringValue = this->AvailableOptionsStringArray[0];
	}
	
	this->TrySetDisplayTextFromStringValue(this->CurrentStringValue);
	
	if (this->DataDynamicSetter)
	{
		this->DataDynamicSetter->SetValueFromString(this->CurrentStringValue);
		
		NotifyListDataModified(this);
	}
}

void UListDataObject_String::BackToPreviousOption()
{
	if (this->AvailableOptionsStringArray.IsEmpty() || this->AvailableOptionsTextArray.IsEmpty()) return;
	
	const int32 CurrentDisplayIndex = this->AvailableOptionsStringArray.IndexOfByKey(this->CurrentStringValue);
	const int32 PreviousIndexToDisplay = CurrentDisplayIndex - 1;
	
	if (this->AvailableOptionsStringArray.IsValidIndex(PreviousIndexToDisplay))
	{
		this->CurrentStringValue = this->AvailableOptionsStringArray[PreviousIndexToDisplay];
	}
	else
	{
		this->CurrentStringValue = this->AvailableOptionsStringArray.Last();
	}
	
	this->TrySetDisplayTextFromStringValue(this->CurrentStringValue);
	
	if (this->DataDynamicSetter)
	{
		this->DataDynamicSetter->SetValueFromString(this->CurrentStringValue);
		
		NotifyListDataModified(this);
	}
}

// special logic for Gamepad
void UListDataObject_String::OnRotatorInitiatedValueChange(const FText& InNewSelectedText)
{
	const int32 FoundIndex = this->AvailableOptionsTextArray.IndexOfByPredicate([InNewSelectedText](const FText& AvailableText)
	{
		return AvailableText.EqualTo(InNewSelectedText);
	});
	
	if (FoundIndex != INDEX_NONE && this->AvailableOptionsStringArray.IsValidIndex(FoundIndex))
	{
		this->CurrentDisplayText = InNewSelectedText;
		this->CurrentStringValue = this->AvailableOptionsStringArray[FoundIndex];
		
		if (this->DataDynamicSetter)
		{
			this->DataDynamicSetter->SetValueFromString(this->CurrentStringValue);
			
			NotifyListDataModified(this);
		}
	}
}

void UListDataObject_String::OnDataObjectInitialized()
{
	if (!this->AvailableOptionsStringArray.IsEmpty()) this->CurrentStringValue = this->AvailableOptionsStringArray[0];
	
	if (HasDefaultValue()) this->CurrentStringValue = GetDefaultValueAsString();
	
	if (this->DataDynamicGetter)
	{
		if (!this->DataDynamicGetter->GetValueAsString().IsEmpty()) this->CurrentStringValue = this->DataDynamicGetter->GetValueAsString();
	}
	
	if (!this->TrySetDisplayTextFromStringValue(this->CurrentStringValue)) this->CurrentDisplayText = FText::FromString(TEXT("Invalid Option"));
}

bool UListDataObject_String::TrySetDisplayTextFromStringValue(const FString& InStringValue)
{
	const int32 CurrentFoundIndex = this->AvailableOptionsStringArray.IndexOfByKey(InStringValue);
	
	if (this->AvailableOptionsTextArray.IsValidIndex(CurrentFoundIndex))
	{
		this->CurrentDisplayText = this->AvailableOptionsTextArray[CurrentFoundIndex];
		
		return true;
	}
	
	return false;
}

bool UListDataObject_String::CanResetBackToDefaultValue() const
{
	return this->HasDefaultValue() && this->CurrentStringValue != GetDefaultValueAsString();
}

bool UListDataObject_String::TryResetBackToDefaultValue()
{
	if (this->CanResetBackToDefaultValue())
	{
		this->CurrentStringValue = GetDefaultValueAsString();
		
		this->TrySetDisplayTextFromStringValue(this->CurrentStringValue);
		
		if (this->DataDynamicSetter)
		{
			this->DataDynamicSetter->SetValueFromString(this->CurrentStringValue);
			NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);
			
			return true;
		}
	}
	
	return false;
}

bool UListDataObject_String::CanSetToForcedStringValue(const FString& InForcedValue) const
{
	return this->CurrentStringValue != InForcedValue;
}

void UListDataObject_String::OnSetToForcedStringValue(const FString& InForcedValue)
{
	this->CurrentStringValue = InForcedValue;
	this->TrySetDisplayTextFromStringValue(this->CurrentStringValue);
	
	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(this->CurrentStringValue);
		
		NotifyListDataModified(this, EOptionsListDataModifyReason::DependencyModified);
	}
}
