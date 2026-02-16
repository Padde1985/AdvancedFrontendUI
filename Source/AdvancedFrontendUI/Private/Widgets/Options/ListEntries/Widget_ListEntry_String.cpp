#include "Widgets/Options/ListEntries/Widget_ListEntry_String.h"

#include "CommonInputSubsystem.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Components/FrontendCommonRotator.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"

void UWidget_ListEntry_String::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);
	
	this->CachedOwningStringDataObject = CastChecked<UListDataObject_String>(InOwningListDataObject);
	
	this->CommonRotator_AvailableOptions->PopulateTextLabels(this->CachedOwningStringDataObject->GetAvailableOptionsTextArray());
	this->CommonRotator_AvailableOptions->SetSelectedOptionByText(this->CachedOwningStringDataObject->GetCurrentDisplayText());
}

void UWidget_ListEntry_String::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	this->CommonButton_PreviousOption->OnClicked().AddUObject(this, &UWidget_ListEntry_String::OnPreviousOptionButtonClicked);
	this->CommonButton_NextOption->OnClicked().AddUObject(this, &UWidget_ListEntry_String::OnNextOptionButtonClicked);
	
	this->CommonRotator_AvailableOptions->OnClicked().AddLambda([this]()
	{
		SelectThisEntryWidget();
	});
	// sepcial logic for the gamepad
	this->CommonRotator_AvailableOptions->OnRotatedEvent.AddUObject(this, &UWidget_ListEntry_String::OnRotatorValueChanged);
}

void UWidget_ListEntry_String::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (this->CachedOwningStringDataObject)
	{
		this->CommonRotator_AvailableOptions->SetSelectedOptionByText(this->CachedOwningStringDataObject->GetCurrentDisplayText());
	}
}

void UWidget_ListEntry_String::OnPreviousOptionButtonClicked()
{
	if (this->CachedOwningStringDataObject)
	{
		this->CachedOwningStringDataObject->BackToPreviousOption();
	}
	
	SelectThisEntryWidget();
}

void UWidget_ListEntry_String::OnNextOptionButtonClicked()
{
	if (this->CachedOwningStringDataObject)
	{
		this->CachedOwningStringDataObject->AdvanceToNextOption();
	}
	
	SelectThisEntryWidget();
}

// special logic for Gamepad
void UWidget_ListEntry_String::OnRotatorValueChanged(int32 Value, bool bUserInitiated)
{
	if (!this->CachedOwningStringDataObject) return;
	
	UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
	
	if (!CommonInputSubsystem || !bUserInitiated) return;
	
	if (CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		this->CachedOwningStringDataObject->OnRotatorInitiatedValueChange(this->CommonRotator_AvailableOptions->GetSelectedText());
	}
}
