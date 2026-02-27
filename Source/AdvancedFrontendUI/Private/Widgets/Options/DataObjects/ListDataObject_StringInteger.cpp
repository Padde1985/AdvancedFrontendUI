#include "Widgets/Options/DataObjects/ListDataObject_StringInteger.h"

#include "Widgets/Options/OptionsDataInteractionHelper.h"

void UListDataObject_StringInteger::AddIntegerOption(int32 InIntegerValue, const FText& InDisplayText)
{
	AddDynamicOption(LexToString(InIntegerValue), InDisplayText);
}

void UListDataObject_StringInteger::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();
	
	if (!TrySetDisplayTextFromStringValue(CurrentStringValue)) CurrentDisplayText = FText::FromString(TEXT("Custom"));
}

void UListDataObject_StringInteger::OnEditDependecyDataModified(UListDataObject_Base* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason)
{
	if (DataDynamicGetter)
	{
		// return if the value is not changed, this will be true when bi-directional dependency is set up
		if (CurrentStringValue == DataDynamicGetter->GetValueAsString()) return;
		
		CurrentStringValue = DataDynamicGetter->GetValueAsString();
		
		if (!TrySetDisplayTextFromStringValue(CurrentStringValue)) CurrentDisplayText = FText::FromString(TEXT("Custom"));
		
		NotifyListDataModified(this, EOptionsListDataModifyReason::DependencyModified);
	}
	
	Super::OnEditDependecyDataModified(ModifiedDependencyData, ModifyReason);
}
