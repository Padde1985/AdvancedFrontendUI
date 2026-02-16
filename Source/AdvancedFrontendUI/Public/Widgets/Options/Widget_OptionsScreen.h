#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_OptionsScreen.generated.h"

class UListDataObject_Base;
class UWidget_OptionsDetailsView;
class UFrontendCommonListView;
class UFrontendTabListWidgetBase;
class UOptionsDataRegistry;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick), PrioritizeCategories=("Frontend Options Screen"))
class ADVANCEDFRONTENDUI_API UWidget_OptionsScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()
public:
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Frontend Options Screen", meta=(RowType = "/Script/CommonUI.CommonInputActionDataBase")) FDataTableRowHandle ResetAction;
	UPROPERTY(Transient) UOptionsDataRegistry* CreatedOwningDataRegistry;
	UPROPERTY(meta=(BindWidget)) UFrontendTabListWidgetBase* TabListWidget_OptionsTabs;
	UPROPERTY(meta=(BindWidget)) UFrontendCommonListView* CommonListView_OptionsList;
	UPROPERTY(meta=(BindWidget)) UWidget_OptionsDetailsView* DetailsView_ListEntryInfo;
	UPROPERTY(Transient) TArray<UListDataObject_Base*> ResettableDataArray;
	
	FUIActionBindingHandle ResetActionHandle;
	bool bIsResettingData = false;
	
	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();
	UOptionsDataRegistry* GetOrCreateDataRegistry();
	void OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered);
	void OnListViewItemSelected(UObject* InSelectedItem);
	FString TryGetEntryWidgetClassName(UObject* InOwningListItem) const;
	void OnListViewListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason);

	UFUNCTION() void OnOptionsTabSelected(FName TabId);
};
