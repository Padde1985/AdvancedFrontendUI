#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "ListDataObject_Base.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "ListDataObject_KeyRemap.generated.h"

class UEnhancedPlayerMappableKeyProfile;
class UEnhancedInputUserSettings;

// class for key binding parameters
UCLASS()
class ADVANCEDFRONTENDUI_API UListDataObject_KeyRemap : public UListDataObject_Base
{
	GENERATED_BODY()
	
public:
	void InitKeyRemapData(UEnhancedInputUserSettings* InSettings, UEnhancedPlayerMappableKeyProfile* InKeyProfile, ECommonInputType InKeyType, const FPlayerKeyMapping& InPlayerKeyMapping);
	FSlateBrush GetIconFromCurrentKey() const;
	ECommonInputType GetDesiredInputKeyType() const;
	void BindNewInputKey(const FKey& InNewKey);
	virtual bool HasDefaultValue() const override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	
private:
	UPROPERTY(Transient) UEnhancedInputUserSettings* CachedOwningUserSettings;
	UPROPERTY(Transient) UEnhancedPlayerMappableKeyProfile* CachedOwningKeyProfile;
	
	ECommonInputType CachedDesiredInputKeyType;
	FName CachedOwningMappingName;
	EPlayerMappableKeySlot CachedOwningMappableKeySlot;
	
	FPlayerKeyMapping* GetOwningKeyMapping() const;
};
