#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

#include "FrontendSettings/FrontendGameUserSettings.h"

FName UListDataObject_Base::GetDataID() const
{
	return this->DataID;
}

void UListDataObject_Base::SetDataID(const FName InDataID)
{
	this->DataID = InDataID;
}

FText UListDataObject_Base::GetDataDisplayName() const
{
	return this->DataDisplayName;
}

void UListDataObject_Base::SetDataDisplayName(const FText InDataDisplayName)
{
	this->DataDisplayName = InDataDisplayName;
}

FText UListDataObject_Base::GetDescriptionRichText() const
{
	return this->DescriptionRichText;
}

void UListDataObject_Base::SetDescriptionRichText(const FText InDescriptionRichText)
{
	this->DescriptionRichText = InDescriptionRichText;
}

FText UListDataObject_Base::GetDisabledRichText() const
{
	return this->DisabledRichText;
}

void UListDataObject_Base::SetDisabledRichText(const FText InDisabledRichText)
{
	this->DisabledRichText = InDisabledRichText;
}

TSoftObjectPtr<UTexture2D> UListDataObject_Base::GetSoftDescriptionImage() const
{
	return this->SoftDescriptionImage;
}

void UListDataObject_Base::SetSoftDescriptionImage(const TSoftObjectPtr<UTexture2D> InSoftDescriptionImage)
{
	this->SoftDescriptionImage = InSoftDescriptionImage;
}

TArray<UListDataObject_Base*> UListDataObject_Base::GetAllChildListData() const
{
	return TArray<UListDataObject_Base*>();
}

bool UListDataObject_Base::HasAnyChildListData() const
{
	return false;
}

void UListDataObject_Base::InitDataObject()
{
	this->OnDataObjectInitialized();
}

void UListDataObject_Base::SetShouldApplaySettingsImmediately(bool bShouldApplayRightAway)
{
	this->bShouldApplayChangeImmediately = bShouldApplayRightAway;
}

bool UListDataObject_Base::HasDefaultValue() const
{
	return false;
}

bool UListDataObject_Base::CanResetBackToDefaultValue() const
{
	return false;
}

bool UListDataObject_Base::TryResetBackToDefaultValue()
{
	return false;
}

void UListDataObject_Base::OnDataObjectInitialized()
{
}

void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	this->OnListDataModified.Broadcast(ModifiedData, ModifyReason);
	
	if (this->bShouldApplayChangeImmediately)
	{
		UFrontendGameUserSettings::Get()->ApplySettings(true);
	}
}

UListDataObject_Base* UListDataObject_Base::GetParentData() const
{
	return this->ParentData;
}

void UListDataObject_Base::SetParentData(UListDataObject_Base* InParentData)
{
	this->ParentData = InParentData;
}
