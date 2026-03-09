#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "Subsystems/FrontendUISubsystem.h"

// set the text to be displayed on a button
void UFrontendCommonButtonBase::SetButtonText(FText InText)
{
	if (this->CommonTextBlock_ButtonText && !InText.IsEmpty())
	{
		this->CommonTextBlock_ButtonText->SetText(this->bUserUpperCaseForButtonText ? InText.ToUpper() : InText);
	}
}

// get current button text
FText UFrontendCommonButtonBase::GetButtonDisplayText() const
{
	if (this->CommonTextBlock_ButtonText) return this->CommonTextBlock_ButtonText->GetText();
	
	return FText();
}

// set an image for the button (e.g. for the List entries where the previous and next buttons are arrows)
void UFrontendCommonButtonBase::SetButtonDisplayImage(const FSlateBrush& InBrush)
{
	if (this->CommonLazyImage_ButtonImage) this->CommonLazyImage_ButtonImage->SetBrush(InBrush);
}

// Pre-construct routine (similar to a Constructor in native classes)
void UFrontendCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	this->SetButtonText(this->ButtonDisplayText);
}

// update the current style used for the button (Common UI uses styles for texts and buttons instead of setting all parameters in the blueprint)
void UFrontendCommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();
	
	if (this->CommonTextBlock_ButtonText && GetCurrentTextStyleClass()) this->CommonTextBlock_ButtonText->SetStyle(GetCurrentTextStyleClass());
}

// broadcast the delegate when hovering a button (updates the details view on the right side of the screen)
void UFrontendCommonButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();
	
	if (!this->ButtonDescriptionText.IsEmpty())
	{
		UFrontendUISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, this->ButtonDescriptionText);
	}
}

// broadcast again but this time basically hide the details text (clear it)
void UFrontendCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	
	UFrontendUISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, FText::GetEmpty());
}