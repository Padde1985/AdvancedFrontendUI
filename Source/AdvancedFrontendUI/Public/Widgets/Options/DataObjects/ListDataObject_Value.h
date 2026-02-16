#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_Base.h"
#include "ListDataObject_Value.generated.h"

class FOptionsDataInteractionHelper;

UCLASS(Abstract)
class ADVANCEDFRONTENDUI_API UListDataObject_Value : public UListDataObject_Base
{
	GENERATED_BODY()
	
public:
	void SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGetter);
	void SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSetter);
	void SetDefaultValueFromString(const FString& InDefaultValue);
	virtual bool HasDefaultValue() const override;
	
protected:
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicGetter;
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicSetter;
	
	FString GetDefaultValueAsString() const;
	
private:
	TOptional<FString> DefaultStringValue;
};
