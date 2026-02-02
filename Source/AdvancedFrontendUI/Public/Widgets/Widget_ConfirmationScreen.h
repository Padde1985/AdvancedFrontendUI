#pragma once

#include "CoreMinimal.h"
#include "Widget_ActivatableBase.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Widget_ConfirmationScreen.generated.h"

class UDynamicEntryBox;
class UCommonTextBlock;

USTRUCT(BlueprintType)
struct FConfirmScreenButtonInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) EConfirmScreenButtonType ConfirmScreenButtonType = EConfirmScreenButtonType::Unknown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText ButtonTextToDisplay;
};

UCLASS()
class ADVANCEDFRONTENDUI_API UConfirmScreenInfoObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient) FText ScreenTitle;
	UPROPERTY(Transient) FText ScreenMessage;
	UPROPERTY(Transient) TArray<FConfirmScreenButtonInfo> AvailableScreenButtons;
	
	static UConfirmScreenInfoObject* CreateOkScreen(const FText& InScreenTitle, const FText& InScreenMessage);
	static UConfirmScreenInfoObject* CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMessage);
	static UConfirmScreenInfoObject* CreateOkCancelScreen(const FText& InScreenTitle, const FText& InScreenMessage);
};

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class ADVANCEDFRONTENDUI_API UWidget_ConfirmationScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()
	
public:
	void InitConfirmScreen(UConfirmScreenInfoObject* InScreenInfoObject, TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback);
	
private:
	UPROPERTY(meta=(BindWidget)) UCommonTextBlock* CommonTextBlock_Title;
	UPROPERTY(meta=(BindWidget)) UCommonTextBlock* CommonTextBlock_Message;
	UPROPERTY(meta=(BindWidget)) UDynamicEntryBox* DynamicEntryBox_Buttons;
};
