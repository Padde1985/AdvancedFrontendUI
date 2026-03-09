#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "FrontendSettings/FrontendGameUserSettings.h"

// getter for Data ID (internal ID)
FName UListDataObject_Base::GetDataID() const
{
	return this->DataID;
}

// setter for Data ID (internal ID)
void UListDataObject_Base::SetDataID(const FName InDataID)
{
	this->DataID = InDataID;
}

// getter for name to be displayed (label of line)
FText UListDataObject_Base::GetDataDisplayName() const
{
	return this->DataDisplayName;
}

// setter for display name (label)
void UListDataObject_Base::SetDataDisplayName(const FText InDataDisplayName)
{
	this->DataDisplayName = InDataDisplayName;
}

// getter for the description text (displayed in the details view on the right side)
FText UListDataObject_Base::GetDescriptionRichText() const
{
	return this->DescriptionRichText;
}

// setter for description text
void UListDataObject_Base::SetDescriptionRichText(const FText InDescriptionRichText)
{
	this->DescriptionRichText = InDescriptionRichText;
}

// getter for disabled text (gets displayed to inform the user why a setting is not modifiable)
FText UListDataObject_Base::GetDisabledRichText() const
{
	return this->DisabledRichText;
}

// set the disabled reason text
void UListDataObject_Base::SetDisabledRichText(const FText InDisabledRichText)
{
	this->DisabledRichText = InDisabledRichText;
}

// getter for any image that has to be displayed in the details pane
TSoftObjectPtr<UTexture2D> UListDataObject_Base::GetSoftDescriptionImage() const
{
	return this->SoftDescriptionImage;
}

// setter for image
void UListDataObject_Base::SetSoftDescriptionImage(const TSoftObjectPtr<UTexture2D> InSoftDescriptionImage)
{
	this->SoftDescriptionImage = InSoftDescriptionImage;
}

// get all children (collections are simple headers and have all the option parameters assigned as children)
TArray<UListDataObject_Base*> UListDataObject_Base::GetAllChildListData() const
{
	return TArray<UListDataObject_Base*>();
}

// check if a header has any parameters assigned as children
bool UListDataObject_Base::HasAnyChildListData() const
{
	return false;
}

// calls the callback function on children (the callback has to be overriden by all child classes as it's empty in the base class)
void UListDataObject_Base::InitDataObject()
{
	this->OnDataObjectInitialized();
}

// setter for flag if settings should be applied immediately when changing it or waiting until a new parameter is selected
void UListDataObject_Base::SetShouldApplaySettingsImmediately(bool bShouldApplayRightAway)
{
	this->bShouldApplayChangeImmediately = bShouldApplayRightAway;
}

// check if a parameter has a default value assigned, otherwise parameter cannot be reset when changed by user
// it's expected to be overriden in all child classes
bool UListDataObject_Base::HasDefaultValue() const
{
	return false;
}

// check if it's allowed to reset a parameter (a parameter can have a default value but may not set back to it automatically)
// it's expected to be overriden in all child classes
bool UListDataObject_Base::CanResetBackToDefaultValue() const
{
	return false;
}

// try to get the default value and set it
// it's expected to be overriden in all child classes
bool UListDataObject_Base::TryResetBackToDefaultValue()
{
	return false;
}

// apply an edit condition (e.g. screen resolution can only be changed in windowed mode but not in fullscreen)
void UListDataObject_Base::AddEditCondition(const FOptionsDataEditConditionDescriptor& InEditCondition)
{
	this->EditConditionDescArray.Add(InEditCondition);
}

// check if a parameter is currently editable and display reason if not
bool UListDataObject_Base::IsDataCurrentlyEditable()
{
	bool bIsEditable = true;
	
	if (this->EditConditionDescArray.IsEmpty()) return bIsEditable;
	
	FString CachedDisabledReason;
	for (const FOptionsDataEditConditionDescriptor& Condition : this->EditConditionDescArray)
	{
		if (!Condition.IsValid() || Condition.IsEditConditionMet()) continue;
		
		bIsEditable = false;
		
		CachedDisabledReason.Append(Condition.GetDisabledRichReason());
		
		this->SetDisabledRichText(FText::FromString(CachedDisabledReason));
		
		if (Condition.HasForcedStringValue())
		{
			const FString ForcedStringValue = Condition.GetDisabledForcedStringValue();
			
			if (this->CanSetToForcedStringValue(ForcedStringValue)) this->OnSetToForcedStringValue(ForcedStringValue);
		}
	}
	
	return bIsEditable;
}

// add an edit dependency (do something when then dependent parameter was changed)
void UListDataObject_Base::AddEditDependencyData(UListDataObject_Base* InEditDependencyData)
{
	if (!InEditDependencyData->OnListDataModified.IsBoundToObject(this)) InEditDependencyData->OnListDataModified.AddUObject(this, &ThisClass::OnEditDependecyDataModified);
}

// do something when the object is initialized
// it's expected to be overriden in all child classes
void UListDataObject_Base::OnDataObjectInitialized()
{
}

// broadcast the delegate when a parameter was changed and save settings
void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	this->OnListDataModified.Broadcast(ModifiedData, ModifyReason);
	
	if (this->bShouldApplayChangeImmediately) UFrontendGameUserSettings::Get()->ApplySettings(true);
}

// check if a parameter can be set to the given value
// it's expected to be overriden in all child classes
bool UListDataObject_Base::CanSetToForcedStringValue(const FString& InForcedValue) const
{
	return false;
}

// callback in the event that a value was set automatically
// it's expected to be overriden in all child classes
void UListDataObject_Base::OnSetToForcedStringValue(const FString& InForcedValue)
{
}

// broadcast delegate in case the dependent object was modified
void UListDataObject_Base::OnEditDependecyDataModified(UListDataObject_Base* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason)
{
	this->OnDependencyDataModified.Broadcast(ModifiedDependencyData, ModifyReason);
}

// get the parent object (collection for list entries, etc)
UListDataObject_Base* UListDataObject_Base::GetParentData() const
{
	return this->ParentData;
}

// set the parent object
void UListDataObject_Base::SetParentData(UListDataObject_Base* InParentData)
{
	this->ParentData = InParentData;
}