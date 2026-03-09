#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"
#include "CommonInputBaseTypes.h"
#include "CommonInputSubsystem.h"
#include "FrontendDebugHelper.h"

// initialize the key bindings object
void UListDataObject_KeyRemap::InitKeyRemapData(UEnhancedInputUserSettings* InSettings, UEnhancedPlayerMappableKeyProfile* InKeyProfile, ECommonInputType InKeyType, const FPlayerKeyMapping& InPlayerKeyMapping)
{
	this->CachedOwningUserSettings = InSettings;
	this->CachedOwningKeyProfile = InKeyProfile;
	this->CachedDesiredInputKeyType = InKeyType;
	this->CachedOwningMappingName = InPlayerKeyMapping.GetMappingName();
	this->CachedOwningMappableKeySlot = InPlayerKeyMapping.GetSlot();
}

// try to retrieve the icon for the current key binding using the data table
FSlateBrush UListDataObject_KeyRemap::GetIconFromCurrentKey() const
{
	check(this->CachedOwningUserSettings);
	
	FSlateBrush FoundBrush;
	UCommonInputSubsystem* CommonInputSubsytem = UCommonInputSubsystem::Get(this->CachedOwningUserSettings->GetLocalPlayer());
	
	check(CommonInputSubsytem);
	
	const bool bHasFoundBrush = UCommonInputPlatformSettings::Get()->TryGetInputBrush(FoundBrush, 
																					this->GetOwningKeyMapping()->GetCurrentKey(), 
																					this->CachedDesiredInputKeyType, 
																					CommonInputSubsytem->GetCurrentGamepadName());
	
	if (!bHasFoundBrush) Debug::Print(TEXT("Unable to find an icon for the key  ") + 
										this->GetOwningKeyMapping()->GetCurrentKey().GetDisplayName().ToString() + 
										TEXT(" Empty Brush was applied"));
	
	return FoundBrush;
}

// getter for the input key type (mouse and keyboard, gamepad, etc)
ECommonInputType UListDataObject_KeyRemap::GetDesiredInputKeyType() const
{
	return this->CachedDesiredInputKeyType;
}

// map a user specified key to this input, save it in the user settings and broadcast the delegate
void UListDataObject_KeyRemap::BindNewInputKey(const FKey& InNewKey)
{
	check(this->CachedOwningUserSettings);
	
	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = this->CachedOwningMappingName;
	KeyArgs.Slot = this->CachedOwningMappableKeySlot;
	KeyArgs.NewKey = InNewKey;
	
	FGameplayTagContainer Container;
	this->CachedOwningUserSettings->MapPlayerKey(KeyArgs, Container);
	this->CachedOwningUserSettings->SaveSettings();
	
	NotifyListDataModified(this);
}

// check if a default key was assigned for the input action
bool UListDataObject_KeyRemap::HasDefaultValue() const
{
	return this->GetOwningKeyMapping()->GetDefaultKey().IsValid();
}

// check if the key binding can be reset to default
bool UListDataObject_KeyRemap::CanResetBackToDefaultValue() const
{
	return this->HasDefaultValue() && this->GetOwningKeyMapping()->IsCustomized();
}

// set key binding back to default and broadcast delegate
bool UListDataObject_KeyRemap::TryResetBackToDefaultValue()
{
	if (this->CanResetBackToDefaultValue())
	{
		check(this->CachedOwningUserSettings);
		
		this->GetOwningKeyMapping()->ResetToDefault();
		
		this->CachedOwningUserSettings->SaveSettings();
		
		NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);
		
		return true;
	}
	
	return false;
}

// retrieve the current key mapping for the input action
FPlayerKeyMapping* UListDataObject_KeyRemap::GetOwningKeyMapping() const
{
	check(this->CachedOwningKeyProfile);
	
	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = this->CachedOwningMappingName;
	KeyArgs.Slot = this->CachedOwningMappableKeySlot;
	
	return this->CachedOwningKeyProfile->FindKeyMapping(KeyArgs);
}