#include "Widgets/Options/Widget_OptionsScreen.h"
#include "ICommonInputModule.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Input/CommonUIInputTypes.h"
#include "Subsystems/FrontendUISubsystem.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Components/FrontendCommonListView.h"
#include "Widgets/Components/FrontendTabListWidgetBase.h"
#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/Widget_OptionsDetailsView.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

// bind callbacks for the option elements, but also for the button bar on the bottom of the screen
void UWidget_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// bind custom button to the action bar
	if (!this->ResetAction.IsNull())
	{
		this->ResetActionHandle = RegisterUIActionBinding(FBindUIActionArgs(
			this->ResetAction, true, FSimpleDelegate::CreateUObject(this, &UWidget_OptionsScreen::OnResetBoundActionTriggered)));
	}
	
	RegisterUIActionBinding(FBindUIActionArgs(
		ICommonInputModule::GetSettings().GetDefaultBackAction(), true, 
		FSimpleDelegate::CreateUObject(this, &UWidget_OptionsScreen::OnBackBoundActionTriggered)));
	
	this->TabListWidget_OptionsTabs->OnTabSelected.AddUniqueDynamic(this, &UWidget_OptionsScreen::OnOptionsTabSelected);
	
	this->CommonListView_OptionsList->OnItemIsHoveredChanged().AddUObject(this, &UWidget_OptionsScreen::OnListViewItemHovered);
	this->CommonListView_OptionsList->OnItemSelectionChanged().AddUObject(this, &UWidget_OptionsScreen::OnListViewItemSelected);
}

// register the options tab itself
void UWidget_OptionsScreen::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	for (UListDataObject_Collection* TabCollection : this->GetOrCreateDataRegistry()->GetRegisteredOptionsTabCollection())
	{
		if (!TabCollection) continue;
		
		const FName TabID = TabCollection->GetDataID();
		if (this->TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabID) != nullptr) continue;
		
		this->TabListWidget_OptionsTabs->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());
	}
}

// gets called once we exit the screen
void UWidget_OptionsScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	
	UFrontendGameUserSettings::Get()->ApplySettings(true);
}

// set the focus on the first item of the list
UWidget* UWidget_OptionsScreen::NativeGetDesiredFocusTarget() const
{
	if (UObject* SelectedObject = this->CommonListView_OptionsList->GetSelectedItem())
	{
		if (UUserWidget* SelectedEntryWidget = this->CommonListView_OptionsList->GetEntryWidgetFromItem(SelectedObject)) return SelectedEntryWidget;
	}
	
	return Super::NativeGetDesiredFocusTarget();
}

// callback for the reset button
void UWidget_OptionsScreen::OnResetBoundActionTriggered()
{
	if (this->ResettableDataArray.IsEmpty()) return;
	
	UCommonButtonBase* SelectedTabButton = this->TabListWidget_OptionsTabs->GetTabButtonBaseByID(this->TabListWidget_OptionsTabs->GetActiveTab());
	const FString SelectedTabButtonName = CastChecked<UFrontendCommonButtonBase>(SelectedTabButton)->GetButtonDisplayText().ToString();
	
	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::YesNo, 
		FText::FromString(TEXT("Reset")), 
		FText::FromString(TEXT("Are you sure you want to reset all the settings under the ") + SelectedTabButtonName + TEXT(" tab.")),
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			if (ClickedButtonType != EConfirmScreenButtonType::Confirmed) return;
			
			// set the status variable to prevent the delegate inside of TryResetBackToDefaultValue to modify the array while we are looping through
			this->bIsResettingData = true;
			bool bHasDataFailedToReset = false;
			
			for (UListDataObject_Base* DataToReset : this->ResettableDataArray)
			{
				if (!DataToReset) continue;
				if (!DataToReset->TryResetBackToDefaultValue())	bHasDataFailedToReset = true;
			}
			
			if (!bHasDataFailedToReset)
			{
				this->ResettableDataArray.Empty();
				RemoveActionBinding(this->ResetActionHandle);
			}
			
			this->bIsResettingData = false;
		});
}

// callback for the back button
void UWidget_OptionsScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}

// singleton for the data registry
UOptionsDataRegistry* UWidget_OptionsScreen::GetOrCreateDataRegistry()
{
	if (!this->CreatedOwningDataRegistry)
	{
		this->CreatedOwningDataRegistry = NewObject<UOptionsDataRegistry>();
		this->CreatedOwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
	}
	
	checkf(this->CreatedOwningDataRegistry, TEXT("Data registry for options screen is not valid"));
	
	return this->CreatedOwningDataRegistry;
}

// callback for hovering an item
void UWidget_OptionsScreen::OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered)
{
	if (!InHoveredItem) return;
	
	UWidget_ListEntry_Base* HoveredEntryWidget = this->CommonListView_OptionsList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(InHoveredItem);
	
	check(HoveredEntryWidget);
	
	HoveredEntryWidget->NativeOnListEntryWidgetHovered(bWasHovered);
	
	if (bWasHovered)
	{
		this->DetailsView_ListEntryInfo->UpdateDetailsViewInfo(CastChecked<UListDataObject_Base>(InHoveredItem), this->TryGetEntryWidgetClassName(InHoveredItem));
	}
	else
	{
		if (UListDataObject_Base* SelectedItem = this->CommonListView_OptionsList->GetSelectedItem<UListDataObject_Base>())
		{
			this->DetailsView_ListEntryInfo->UpdateDetailsViewInfo(SelectedItem, this->TryGetEntryWidgetClassName(SelectedItem));
		}
	}
}

// callback for selecting an item
void UWidget_OptionsScreen::OnListViewItemSelected(UObject* InSelectedItem)
{
	if (!InSelectedItem) return;
	
	this->DetailsView_ListEntryInfo->UpdateDetailsViewInfo(CastChecked<UListDataObject_Base>(InSelectedItem), this->TryGetEntryWidgetClassName(InSelectedItem));
}

// get the class name of the row type in the list view
FString UWidget_OptionsScreen::TryGetEntryWidgetClassName(UObject* InOwningListItem) const
{
	if (UUserWidget* FoundEntryWidget = this->CommonListView_OptionsList->GetEntryWidgetFromItem(InOwningListItem))
	{
		return FoundEntryWidget->GetClass()->GetName();
	}
	
	return TEXT("Entry Widget not valid");
}

// callback when the data should be reset
void UWidget_OptionsScreen::OnListViewListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (!ModifiedData || this->bIsResettingData) return;
	
	if (ModifiedData->CanResetBackToDefaultValue())
	{
		this->ResettableDataArray.AddUnique(ModifiedData);
		
		if (!GetActionBindings().Contains(this->ResetActionHandle)) AddActionBinding(this->ResetActionHandle);
	}
	else
	{
		if (this->ResettableDataArray.Contains(ModifiedData)) this->ResettableDataArray.Remove(ModifiedData);
	}
	
	if (this->ResettableDataArray.IsEmpty()) RemoveActionBinding(this->ResetActionHandle);
}

// activate one of the options tabs
void UWidget_OptionsScreen::OnOptionsTabSelected(FName TabId)
{
	this->DetailsView_ListEntryInfo->ClearDetailsViewInfo();
	
	TArray<UListDataObject_Base*> FoundListSourceItems = this->GetOrCreateDataRegistry()->GetListSourceItemsBySelectedTabID(TabId);
	this->CommonListView_OptionsList->SetListItems(FoundListSourceItems);
	this->CommonListView_OptionsList->RequestRefresh();
	
	if (this->CommonListView_OptionsList->GetNumItems() != 0)
	{
		this->CommonListView_OptionsList->NavigateToIndex(0);
		this->CommonListView_OptionsList->SetSelectedIndex(0);
	}
	
	this->ResettableDataArray.Empty();
	for (UListDataObject_Base* FoundListSourceItem : FoundListSourceItems)
	{
		if (!FoundListSourceItem) continue;
		
		if (!FoundListSourceItem->OnListDataModified.IsBoundToObject(this))	FoundListSourceItem->OnListDataModified.AddUObject(this, &UWidget_OptionsScreen::OnListViewListDataModified);
		
		if (FoundListSourceItem->CanResetBackToDefaultValue()) this->ResettableDataArray.AddUnique(FoundListSourceItem);
	}
	
	// if no entries found, hide the reset button
	if (this->ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(this->ResetActionHandle);
	}
	else
	{
		if (!GetActionBindings().Contains(this->ResetActionHandle)) AddActionBinding(this->ResetActionHandle);
	}
}