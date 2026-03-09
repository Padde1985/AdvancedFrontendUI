#include "Widgets/Options/Widget_OptionsDetailsView.h"
#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

// update the information shown in the right pane
void UWidget_OptionsDetailsView::UpdateDetailsViewInfo(UListDataObject_Base* InDataObject, const FString& InEntryWidgetClassName)
{
	if (!InDataObject) return;
	
	this->CommonTextBlock_Title->SetText(InDataObject->GetDataDisplayName());
	
	if (!InDataObject->GetSoftDescriptionImage().IsNull())
	{
		this->CommonLazyImage_DescriptionImage->SetBrushFromLazyTexture(InDataObject->GetSoftDescriptionImage());
		this->CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		this->CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	this->CommonRichText_Description->SetText(InDataObject->GetDescriptionRichText());
	
	const FString DynamicDetails = FString::Printf(TEXT("Data Object Class: <Bold>%s</>\n\nEntry Widget Class: <Bold>%s</>"), *InDataObject->GetClass()->GetName(), *InEntryWidgetClassName);
	this->CommonRichText_DynamicDetails->SetText(FText::FromString(DynamicDetails));
	
	this->CommonRichText_DisabledReason->SetText(InDataObject->IsDataCurrentlyEditable() ? FText::GetEmpty() : InDataObject->GetDisabledRichText());
}

// empty the details view when deselecting an option
void UWidget_OptionsDetailsView::ClearDetailsViewInfo() const
{
	this->CommonTextBlock_Title->SetText(FText::GetEmpty());
	this->CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::Collapsed);
	this->CommonRichText_Description->SetText(FText::GetEmpty());
	this->CommonRichText_DynamicDetails->SetText(FText::GetEmpty());
	this->CommonRichText_DisabledReason->SetText(FText::GetEmpty());
}

// initialize the details pane (show an empty pane)
void UWidget_OptionsDetailsView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	this->ClearDetailsViewInfo();
}