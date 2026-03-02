#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

void UWidget_ListEntry_Base::NativeOnListEntryWidgetHovered(bool bWasHovered)
{
	this->BP_OnListEntryWidgetHovered(bWasHovered, GetListItem() ? IsListItemSelected() : false);
	
	if (bWasHovered)
	{
		this->BP_OnToggleEntryWidgetHighlightState(true);
	}
	else
	{
		this->BP_OnToggleEntryWidgetHighlightState(GetListItem() && IsListItemSelected() ? true : false);
	}
}

// will be called any time the object is displayed on the screen
void UWidget_ListEntry_Base::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	
	this->OnOwningListDataObjectSet(CastChecked<UListDataObject_Base>(ListItemObject));
}

void UWidget_ListEntry_Base::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	if (this->CommonText_SettingDisplayName)
	{
		this->CommonText_SettingDisplayName->SetText(InOwningListDataObject->GetDataDisplayName());
	}
	
	if (!InOwningListDataObject->OnListDataModified.IsBoundToObject(this)) InOwningListDataObject->OnListDataModified.AddUObject(this, &UWidget_ListEntry_Base::OnOwningListDataObjectModified);
	if (!InOwningListDataObject->OnDependencyDataModified.IsBoundToObject(this)) InOwningListDataObject->OnDependencyDataModified.AddUObject(this, &ThisClass::OnOwningDependencyDataObjectModified);
	
	this->OnToggleEditableState(InOwningListDataObject->IsDataCurrentlyEditable());
	
	this->CachedOwningDataObject = InOwningListDataObject;
}

void UWidget_ListEntry_Base::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
}

void UWidget_ListEntry_Base::SelectThisEntryWidget()
{
	CastChecked<UListView>(GetOwningListView())->SetSelectedItem(GetListItem());
}

FReply UWidget_ListEntry_Base::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	// special logic for gamepad interaction
	if (UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem(); CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		if (UWidget* WidgetToFocus = this->BP_GetWidgetToFocusForGamepad())
		{
			if (TSharedPtr<SWidget> SlateWidgetToFocus = WidgetToFocus->GetCachedWidget()) return FReply::Handled().SetUserFocus(SlateWidgetToFocus.ToSharedRef());
		}
	}
	
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UWidget_ListEntry_Base::NativeOnEntryReleased()
{
	IUserObjectListEntry::NativeOnEntryReleased();
	
	this->NativeOnListEntryWidgetHovered(false);
}

void UWidget_ListEntry_Base::OnToggleEditableState(bool bIsEditable)
{
	if (this->CommonText_SettingDisplayName) this->CommonText_SettingDisplayName->SetIsEnabled(bIsEditable);
}

void UWidget_ListEntry_Base::OnOwningDependencyDataObjectModified(UListDataObject_Base* OwningModifiedDependecyData, EOptionsListDataModifyReason ModifyReason)
{
	if (this->CachedOwningDataObject) this->OnToggleEditableState(this->CachedOwningDataObject->IsDataCurrentlyEditable());
}

void UWidget_ListEntry_Base::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
	
	this->BP_OnToggleEntryWidgetHighlightState(bIsSelected);
}
