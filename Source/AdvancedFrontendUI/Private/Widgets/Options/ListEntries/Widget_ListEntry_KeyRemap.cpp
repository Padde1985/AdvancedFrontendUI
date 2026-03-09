#include "Widgets/Options/ListEntries/Widget_ListEntry_KeyRemap.h"
#include "FrontendFunctionLibrary.h"
#include "FrontendGameplayTags.h"
#include "Subsystems/FrontendUISubsystem.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Options/Widget_KeyRemapScreen.h"
#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"

// callback when the owning data object is set
void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);
	
	this->CachedOwningKeyRemapDataObject = CastChecked<UListDataObject_KeyRemap>(InOwningListDataObject);
	this->CommonButton_RemapKey->SetButtonDisplayImage(this->CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
}

// callback when the data object gets modified
void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{	
	if (this->CachedOwningKeyRemapDataObject) this->CommonButton_RemapKey->SetButtonDisplayImage(this->CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
}

// bind button callbacks
void UWidget_ListEntry_KeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	this->CommonButton_RemapKey->OnClicked().AddUObject(this, &ThisClass::OnRemapKeyButtonClicked);
	this->CommonButton_ResetKeyBinding->OnClicked().AddUObject(this, &ThisClass::OnResetKeyBindingButtonClicked);
}

// call the process for rebinding a key mapping
void UWidget_ListEntry_KeyRemap::OnRemapKeyButtonClicked()
{
	SelectThisEntryWidget();
	
	UFrontendUISubsystem::Get(this)->PushSoftWidgetToStackAsync(FrontendGameplayTags::Frontend_WidgetStack_Modal, 
																				UFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(FrontendGameplayTags::Frontend_Widget_KeyRemapScreen),
																				[this](EAsyncPushWidgetState PushState, UWidget_ActivatableBase* PushedWidget)
	{
		if (PushState == EAsyncPushWidgetState::OnCreatedBeforePush)
		{
			UWidget_KeyRemapScreen* CreatedKeyRemapScreen = CastChecked<UWidget_KeyRemapScreen>(PushedWidget);
			CreatedKeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(this, &ThisClass::OnKeyToRemapPressed);
			CreatedKeyRemapScreen->OnKeyRemapScreenKeySelectCanceled.BindUObject(this, &ThisClass::OnKeyRemapCanceled);
			
			if (this->CachedOwningKeyRemapDataObject)
			{
				CreatedKeyRemapScreen->SetDesiredInputTypeToFilter(this->CachedOwningKeyRemapDataObject->GetDesiredInputKeyType());
			}
		}
	});
}

// reset the key binding back to default if possible
void UWidget_ListEntry_KeyRemap::OnResetKeyBindingButtonClicked()
{
	SelectThisEntryWidget();
	
	if (!this->CachedOwningKeyRemapDataObject) return;
	
	// check if current key is already the default key
	if (!this->CachedOwningKeyRemapDataObject->CanResetBackToDefaultValue())
	{
		UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
			EConfirmScreenType::OK, 
			FText::FromString(TEXT("Reset Key Mapping")), 
			FText::FromString(TEXT("Key Binding for ") + this->CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT("is already the Default value")),
			[](EConfirmScreenButtonType ClickedButton) {}
		);
		
		return;
	}
	
	// reset the key binding back to default
	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
			EConfirmScreenType::YesNo, 
			FText::FromString(TEXT("Reset Key Mapping")), 
			FText::FromString(TEXT("Are you sure you want to reset the key binding for ") + this->CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT("?")),
			[this](EConfirmScreenButtonType ClickedButton)
			{
				if (ClickedButton == EConfirmScreenButtonType::Confirmed)
				{
					this->CachedOwningKeyRemapDataObject->TryResetBackToDefaultValue();
				}
			}
		);
}

// callback to receive the pressed key during key remapping
void UWidget_ListEntry_KeyRemap::OnKeyToRemapPressed(const FKey& PressedKey)
{
	if (this->CachedOwningKeyRemapDataObject) this->CachedOwningKeyRemapDataObject->BindNewInputKey(PressedKey);
}

// callback in case the key remapping is canceled
void UWidget_ListEntry_KeyRemap::OnKeyRemapCanceled(const FString& CanceledReason)
{
	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::OK, 
		FText::FromString(TEXT("Key Remap")), 
		FText::FromString(CanceledReason),
		[](EConfirmScreenButtonType ClickedButton){});
}