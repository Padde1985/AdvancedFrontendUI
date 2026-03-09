#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Widget_ListEntry_Base.generated.h"

class UListDataObject_Base;
class UCommonTextBlock;

// base class for the User interface widget blueprints
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class ADVANCEDFRONTENDUI_API UWidget_ListEntry_Base : public UCommonUserWidget, public IUserObjectListEntry //<- this one is needed to show up in the entry widget class field in the Widget Blueprint
{
	GENERATED_BODY()
	
public:
	void NativeOnListEntryWidgetHovered(bool bWasHovered);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On List Entry Widget Hovered")) void BP_OnListEntryWidgetHovered(bool bWasHovered, bool bIsEntryWidgetStillSelected);
	
protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject);
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason);
	void SelectThisEntryWidget();
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnEntryReleased() override;
	virtual void OnToggleEditableState(bool bIsEditable);
	virtual void OnOwningDependencyDataObjectModified(UListDataObject_Base* OwningModifiedDependecyData, EOptionsListDataModifyReason ModifyReason);
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "Get Widget To Focus For Gamepad")) UWidget* BP_GetWidgetToFocusForGamepad() const;
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "On Toggle Entry Widget Highlight State")) void BP_OnToggleEntryWidgetHighlightState(bool bShouldHighlight) const;
	
private:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess = "true")) UCommonTextBlock* CommonText_SettingDisplayName;
	UPROPERTY(Transient) UListDataObject_Base* CachedOwningDataObject;
};
