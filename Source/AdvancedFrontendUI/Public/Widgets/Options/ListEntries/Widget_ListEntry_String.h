#pragma once

#include "CoreMinimal.h"
#include "Widget_ListEntry_Base.h"
#include "Widget_ListEntry_String.generated.h"

class UListDataObject_String;
class UFrontendCommonRotator;
class UFrontendCommonButtonBase;

// graphical representation of the standard layout (two buttons with a rotator in between)
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class ADVANCEDFRONTENDUI_API UWidget_ListEntry_String : public UWidget_ListEntry_Base
{
	GENERATED_BODY()
	
protected:
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	virtual void NativeOnInitialized() override;
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;
	virtual void OnToggleEditableState(bool bIsEditable) override;
	
private:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true")) UFrontendCommonButtonBase* CommonButton_PreviousOption;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true")) UFrontendCommonRotator* CommonRotator_AvailableOptions;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true")) UFrontendCommonButtonBase* CommonButton_NextOption;
	UPROPERTY(Transient) UListDataObject_String* CachedOwningStringDataObject;
	
	void OnPreviousOptionButtonClicked();
	void OnNextOptionButtonClicked();
	void OnRotatorValueChanged(int32 Value, bool bUserInitiated);
};
