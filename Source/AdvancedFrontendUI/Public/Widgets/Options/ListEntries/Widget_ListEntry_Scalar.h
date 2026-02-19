#pragma once

#include "CoreMinimal.h"
#include "Widget_ListEntry_Base.h"
#include "Widget_ListEntry_Scalar.generated.h"

class UAnalogSlider;
class UCommonNumericTextBlock;
class UListDataObject_Scalar;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class ADVANCEDFRONTENDUI_API UWidget_ListEntry_Scalar : public UWidget_ListEntry_Base
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;
	
private:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true")) UCommonNumericTextBlock* CommonNumeric_SettingValue;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true")) UAnalogSlider* AnalogSlider_SettingSlider;
	UPROPERTY(Transient) UListDataObject_Scalar* CachedOwningScalarDataObject;

	UFUNCTION() void OnSliderValueChanged(float Value);
	UFUNCTION() void OnSliderMouseCaptureBegin();
};
