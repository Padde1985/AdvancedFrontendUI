#include "Widgets/Widget_ConfirmationScreen.h"

#include "CommonTextBlock.h"
#include "ICommonInputModule.h"
#include "Components/DynamicEntryBox.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkScreen(const FText& InScreenTitle, const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;
	
	FConfirmScreenButtonInfo OkButtonInfo;
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Closed;
	OkButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("OK"));
	
	InfoObject->AvailableScreenButtons.Add(OkButtonInfo);
	
	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;
	
	FConfirmScreenButtonInfo YesButtonInfo;
	YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	YesButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Yes"));
	InfoObject->AvailableScreenButtons.Add(YesButtonInfo);
	
	FConfirmScreenButtonInfo NoButtonInfo;
	NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	NoButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("No"));
	InfoObject->AvailableScreenButtons.Add(NoButtonInfo);
	
	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkCancelScreen(const FText& InScreenTitle, const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;
	
	FConfirmScreenButtonInfo OkButtonInfo;
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	OkButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Ok"));
	InfoObject->AvailableScreenButtons.Add(OkButtonInfo);
	
	FConfirmScreenButtonInfo CancelButtonInfo;
	CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	CancelButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Cancel"));
	InfoObject->AvailableScreenButtons.Add(CancelButtonInfo);
	
	return InfoObject;
}

void UWidget_ConfirmationScreen::InitConfirmScreen(UConfirmScreenInfoObject* InScreenInfoObject, TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback)
{
	check(InScreenInfoObject && this->CommonTextBlock_Title && this->CommonTextBlock_Message && this->DynamicEntryBox_Buttons);
	
	this->CommonTextBlock_Title->SetText(InScreenInfoObject->ScreenTitle);
	this->CommonTextBlock_Message->SetText(InScreenInfoObject->ScreenMessage);
	
	if (this->DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		// clearing old entries, the widget type is specified in the child blueprint
		this->DynamicEntryBox_Buttons->Reset<UFrontendCommonButtonBase>([](UFrontendCommonButtonBase& ExistingButton)
		{
			ExistingButton.OnClicked().Clear();
		});
	}
	
	check(!InScreenInfoObject->AvailableScreenButtons.IsEmpty());
	
	for (const FConfirmScreenButtonInfo& Button : InScreenInfoObject->AvailableScreenButtons)
	{
		FDataTableRowHandle InputActionRowHandle;
		
		switch (Button.ConfirmScreenButtonType)
		{
		case EConfirmScreenButtonType::Confirmed: // Common UI is handling the confirmation automatically
			break;
		case EConfirmScreenButtonType::Cancelled:
			InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
			break;
		case EConfirmScreenButtonType::Closed:
			InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
			break;
		case EConfirmScreenButtonType::Unknown:
			break;
		}
		
		UFrontendCommonButtonBase* AddedButton = this->DynamicEntryBox_Buttons->CreateEntry<UFrontendCommonButtonBase>();
		AddedButton->SetButtonText(Button.ButtonTextToDisplay);
		AddedButton->SetTriggeringInputAction(InputActionRowHandle);
		AddedButton->OnClicked().AddLambda([ClickedButtonCallback, Button, this]()
		{
			ClickedButtonCallback(Button.ConfirmScreenButtonType);
			DeactivateWidget();
		});
	}
	
	if (this->DynamicEntryBox_Buttons->GetNumEntries() != 0) this->DynamicEntryBox_Buttons->GetAllEntries().Last()->SetFocus();
}