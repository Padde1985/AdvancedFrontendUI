#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_KeyRemapScreen.generated.h"

class FKeyRemapScreenInputPreprocessor;
class UCommonRichTextBlock;

DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeyPressedDelegate, const FKey& /*PressedKey*/);
DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeySelectCanceledDelegate, const FString& /*CanceledReason*/);

// user interface widget class
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class ADVANCEDFRONTENDUI_API UWidget_KeyRemapScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()
	
public:
	FOnKeyRemapScreenKeyPressedDelegate OnKeyRemapScreenKeyPressed;
	FOnKeyRemapScreenKeySelectCanceledDelegate OnKeyRemapScreenKeySelectCanceled;
	
	void SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType);
	
protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	
private:
	UPROPERTY(meta=(BindWidget)) UCommonRichTextBlock* CommonRichText_RemapMessage;
	
	TSharedPtr<FKeyRemapScreenInputPreprocessor> CachedInputPreprocessor;
	ECommonInputType CachedDesiredInputType;
	
	void OnValidKeyPressedDetected(const FKey& PressedKey);
	void OnKeySelectCanceled(const FString& CanceledReason);
	void RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback);
};
