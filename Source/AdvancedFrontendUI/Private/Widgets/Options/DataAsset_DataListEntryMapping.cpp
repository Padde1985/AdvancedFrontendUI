#include "Widgets/Options/DataAsset_DataListEntryMapping.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

// find the widget class for the given data object, the found widget can even be a parent widget class
TSubclassOf<UWidget_ListEntry_Base> UDataAsset_DataListEntryMapping::FindEntryWidgetClassByDataObject(UListDataObject_Base* InDataObject) const
{
	check(InDataObject);
	
	// looks strange, but works
	// essentially, recursive looping and starting with the class of the input parameter, checking if in Map, otherwise try again with superclass and superclass of superclass, etc.
	for (UClass* DataObjectClass = InDataObject->GetClass(); DataObjectClass; DataObjectClass = DataObjectClass->GetSuperClass())
	{
		if (TSubclassOf<UListDataObject_Base> ConvertedDataObjectClass = TSubclassOf<UListDataObject_Base>(DataObjectClass))
		{
			if (this->DataObjectListEntryMap.Contains(ConvertedDataObjectClass)) return this->DataObjectListEntryMap.FindRef(ConvertedDataObjectClass);
		}
	}
	
	return TSubclassOf<UWidget_ListEntry_Base>();
}
