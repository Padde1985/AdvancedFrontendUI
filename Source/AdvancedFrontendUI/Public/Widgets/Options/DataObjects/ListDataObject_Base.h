#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "ListDataObject_Base.generated.h"

class UListDataObject_Base;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModifiedDelegate, UListDataObject_Base*, EOptionsListDataModifyReason);

UCLASS(Abstract)
class ADVANCEDFRONTENDUI_API UListDataObject_Base : public UObject
{
	GENERATED_BODY()
	
public:
	FOnListDataModifiedDelegate OnListDataModified;
	
	FName GetDataID() const;
	void SetDataID(const FName InDataID);
	FText GetDataDisplayName() const;
	void SetDataDisplayName(const FText InDataDisplayName);
	FText GetDescriptionRichText() const;
	void SetDescriptionRichText(const FText InDescriptionRichText);
	FText GetDisabledRichText() const;
	void SetDisabledRichText(const FText InDisabledRichText);
	TSoftObjectPtr<UTexture2D> GetSoftDescriptionImage() const;
	void SetSoftDescriptionImage(const TSoftObjectPtr<UTexture2D> InSoftDescriptionImage);
	UListDataObject_Base* GetParentData() const;
	void SetParentData(UListDataObject_Base* InParentData);
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const;
	virtual bool HasAnyChildListData() const;
	void InitDataObject();
	void SetShouldApplaySettingsImmediately(bool bShouldApplayRightAway);
	virtual bool HasDefaultValue() const;
	virtual bool CanResetBackToDefaultValue() const;
	virtual bool TryResetBackToDefaultValue();
	
protected:
	virtual void OnDataObjectInitialized();
	virtual void NotifyListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified);

private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;
	bool bShouldApplayChangeImmediately = false;

	UPROPERTY(Transient) UListDataObject_Base* ParentData;
};
