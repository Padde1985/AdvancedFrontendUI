#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"

#include "CommonInputBaseTypes.h"
#include "CommonInputSubsystem.h"
#include "FrontendDebugHelper.h"

void UListDataObject_KeyRemap::InitKeyRemapData(UEnhancedInputUserSettings* InSettings, UEnhancedPlayerMappableKeyProfile* InKeyProfile, ECommonInputType InKeyType, const FPlayerKeyMapping& InPlayerKeyMapping)
{
	this->CachedOwningUserSettings = InSettings;
	this->CachedOwningKeyProfile = InKeyProfile;
	this->CachedDesiredInputKeyType = InKeyType;
	this->CachedOwningMappingName = InPlayerKeyMapping.GetMappingName();
	this->CachedOwningMappableKeySlot = InPlayerKeyMapping.GetSlot();
}

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

ECommonInputType UListDataObject_KeyRemap::GetDesiredInputKeyType() const
{
	return this->CachedDesiredInputKeyType;
}

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

bool UListDataObject_KeyRemap::HasDefaultValue() const
{
	return this->GetOwningKeyMapping()->GetDefaultKey().IsValid();
}

bool UListDataObject_KeyRemap::CanResetBackToDefaultValue() const
{
	return this->HasDefaultValue() && this->GetOwningKeyMapping()->IsCustomized();
}

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

FPlayerKeyMapping* UListDataObject_KeyRemap::GetOwningKeyMapping() const
{
	check(this->CachedOwningKeyProfile);
	
	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = this->CachedOwningMappingName;
	KeyArgs.Slot = this->CachedOwningMappableKeySlot;
	
	return this->CachedOwningKeyProfile->FindKeyMapping(KeyArgs);
}