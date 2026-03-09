#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"
#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

// hover callback, calls two blueprint events
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

// callback when the owner of this option (Data list object) is set
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

// callback when the data object was modified
// expected to be overriden by all child classes
void UWidget_ListEntry_Base::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
}

// set focus on this widget, needed to navigate via gamepad or keyboard
void UWidget_ListEntry_Base::SelectThisEntryWidget()
{
	CastChecked<UListView>(GetOwningListView())->SetSelectedItem(GetListItem());
}

// callback when the element receives the focus (hovered or clicked on)
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

// callback when dehovered
void UWidget_ListEntry_Base::NativeOnEntryReleased()
{
	IUserObjectListEntry::NativeOnEntryReleased();
	
	this->NativeOnListEntryWidgetHovered(false);
}

// change the editable property of the option
void UWidget_ListEntry_Base::OnToggleEditableState(bool bIsEditable)
{
	if (this->CommonText_SettingDisplayName) this->CommonText_SettingDisplayName->SetIsEnabled(bIsEditable);
}

// callback when the dependent data object is modified
void UWidget_ListEntry_Base::OnOwningDependencyDataObjectModified(UListDataObject_Base* OwningModifiedDependecyData, EOptionsListDataModifyReason ModifyReason)
{
	if (this->CachedOwningDataObject) this->OnToggleEditableState(this->CachedOwningDataObject->IsDataCurrentlyEditable());
}

// callback when the item is clicked on
void UWidget_ListEntry_Base::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
	
	this->BP_OnToggleEntryWidgetHighlightState(bIsSelected);
}