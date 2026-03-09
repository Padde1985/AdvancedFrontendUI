#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"

// get all assigned children
TArray<UListDataObject_Base*> UListDataObject_Collection::GetAllChildListData() const
{
	return this->ChildListDataArray;
}

// check if any parameters are assigned to this collection
bool UListDataObject_Collection::HasAnyChildListData() const
{
	return !this->ChildListDataArray.IsEmpty();
}

// add new children and set the parent object to this object here
void UListDataObject_Collection::AddChildListData(UListDataObject_Base* InChildListData)
{
	InChildListData->InitDataObject();
	InChildListData->SetParentData(this);
	
	this->ChildListDataArray.Add(InChildListData);
}