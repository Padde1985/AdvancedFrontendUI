#include "Widgets/Components/FrontendCommonButtonBase.h"

#include "CommonTextBlock.h"
#include "Subsystems/FrontendUISubsystem.h"

void UFrontendCommonButtonBase::SetButtonText(FText InText)
{
	if (this->CommonTextBlock_ButtonText && !InText.IsEmpty())
	{
		this->CommonTextBlock_ButtonText->SetText(this->bUserUpperCaseForButtonText ? InText.ToUpper() : InText);
	}
}

void UFrontendCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	this->SetButtonText(this->ButtonDisplayText);
}

void UFrontendCommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();
	
	if (this->CommonTextBlock_ButtonText && GetCurrentTextStyleClass()) this->CommonTextBlock_ButtonText->SetStyle(GetCurrentTextStyleClass());
}

void UFrontendCommonButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();
	
	if (!this->ButtonDescriptionText.IsEmpty())
	{
		UFrontendUISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, this->ButtonDescriptionText);
	}
}

void UFrontendCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	
	UFrontendUISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, FText::GetEmpty());
}
