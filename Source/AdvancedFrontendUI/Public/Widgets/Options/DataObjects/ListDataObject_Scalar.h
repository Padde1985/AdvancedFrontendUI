#pragma once

#include "CoreMinimal.h"
#include "CommonNumericTextBlock.h"
#include "ListDataObject_Value.h"
#include "ListDataObject_Scalar.generated.h"

// scalar class for the slider representation (e.g. brightness)
UCLASS()
class ADVANCEDFRONTENDUI_API UListDataObject_Scalar : public UListDataObject_Value
{
	GENERATED_BODY()

public:
	TRange<float> GetDisplayValueRange() const;
	void SetDisplayValueRange(const TRange<float> InDisplayValueRange);
	TRange<float> GetOutputValueRange() const;
	void SetOutputValueRange(const TRange<float> InOutputValueRange);
	float GetSliderStepSize() const;
	void SetSliderStepSize(float InSliderStepSize);
	ECommonNumericType GetDisplayNumericType() const;
	void SetDisplayNumericType(ECommonNumericType InDisplayNumericType);
	FCommonNumberFormattingOptions GetNumberFormattingOptions() const;
	void SetNumberFormattingOptions(const FCommonNumberFormattingOptions InNumberFormattingOptions);
	static FCommonNumberFormattingOptions NoDecimal();
	static FCommonNumberFormattingOptions WithDecimal(int32 NumFracDigits);
	float GetCurrentValue() const;
	void SetCurrentValueFromSlider(float InNewValue);
	
private:
	TRange<float> DisplayValueRange = TRange<float>(0.f, 1.f);
	TRange<float> OutputValueRange = TRange<float>(0.f, 1.f);
	float SliderStepSize = 0.1f;
	ECommonNumericType DisplayNumericType = ECommonNumericType::Number;
	FCommonNumberFormattingOptions NumberFormattingOptions;
	
	float StringToFloat(const FString& InString) const;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	virtual void OnEditDependecyDataModified(UListDataObject_Base* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified) override;
};
