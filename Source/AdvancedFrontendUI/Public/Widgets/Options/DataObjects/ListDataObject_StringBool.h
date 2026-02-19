#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_String.h"
#include "ListDataObject_StringBool.generated.h"

UCLASS()
class ADVANCEDFRONTENDUI_API UListDataObject_StringBool : public UListDataObject_String
{
	GENERATED_BODY()
	
public:
	void OverrideTrueDisplayText(const FText& InNewTrueDisplayText);
	void OverrideFalseDisplayText(const FText& InNewFalseDisplayText);
	void SetTrueAsDefaultValue();
	void SetFalseAsDefaultValue();
	
protected:
	virtual void OnDataObjectInitialized() override;
	
private:
	const FString TrueString = TEXT("True");
	const FString FalseString = TEXT("False");
	
	void TryInitBoolValues();
};
