#pragma once

#include "FrontendStructTypes.generated.h"

USTRUCT()
struct FOptionsDataEditConditionDescriptor
{
	GENERATED_BODY()
	
public:
	void SetEditConditionFunc(TFunction<bool()> InEditConditionFunc);
	bool IsValid() const;
	bool IsEditConditionMet() const;
	FString GetDisabledRichReason() const;
	void SetDisabledRichReason(const FString& InDisabledRichReason);
	bool HasForcedStringValue() const;
	FString GetDisabledForcedStringValue() const;
	void SetDisabledForcedStringValue(const FString& InForcedValue);
	
private:
	TFunction<bool()> EditConditionFunc;
	FString DisabledRichReason;
	TOptional<FString> DisabledForcedStringValue;
};