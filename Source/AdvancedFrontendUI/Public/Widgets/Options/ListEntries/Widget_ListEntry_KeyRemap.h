#pragma once

#include "CoreMinimal.h"
#include "Widget_ListEntry_Base.h"
#include "Widget_ListEntry_KeyRemap.generated.h"

class UListDataObject_KeyRemap;
class UFrontendCommonButtonBase;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class ADVANCEDFRONTENDUI_API UWidget_ListEntry_KeyRemap : public UWidget_ListEntry_Base
{
	GENERATED_BODY()
	
protected:
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true")) UFrontendCommonButtonBase* CommonButton_RemapKey;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true")) UFrontendCommonButtonBase* CommonButton_ResetKeyBinding;
	UPROPERTY(Transient) UListDataObject_KeyRemap* CachedOwningKeyRemapDataObject;
	
	void OnRemapKeyButtonClicked();
	void OnResetKeyBindingButtonClicked();
	void OnKeyToRemapPressed(const FKey& PressedKey);
	void OnKeyRemapCanceled(const FString& CanceledReason);
};
