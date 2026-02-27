#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_String.h"
#include "ListDataObject_StringInteger.generated.h"

UCLASS()
class ADVANCEDFRONTENDUI_API UListDataObject_StringInteger : public UListDataObject_String
{
	GENERATED_BODY()
	
public:
	void AddIntegerOption(int32 InIntegerValue, const FText& InDisplayText);
	
protected:
	virtual void OnDataObjectInitialized() override;
	virtual void OnEditDependecyDataModified(UListDataObject_Base* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified) override;
};
