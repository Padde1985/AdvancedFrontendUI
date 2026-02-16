#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_OptionsDetailsView.generated.h"

class UListDataObject_Base;
class UCommonRichTextBlock;
class UCommonLazyImage;
class UCommonTextBlock;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class ADVANCEDFRONTENDUI_API UWidget_OptionsDetailsView : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateDetailsViewInfo(UListDataObject_Base* InDataObject, const FString& InEntryWidgetClassName = FString());
	void ClearDetailsViewInfo() const;

protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta=(BindWidget)) UCommonTextBlock* CommonTextBlock_Title;
	UPROPERTY(meta=(BindWidget)) UCommonLazyImage* CommonLazyImage_DescriptionImage;
	UPROPERTY(meta=(BindWidget)) UCommonRichTextBlock* CommonRichText_Description;
	UPROPERTY(meta=(BindWidget)) UCommonRichTextBlock* CommonRichText_DynamicDetails;
	UPROPERTY(meta=(BindWidget)) UCommonRichTextBlock* CommonRichText_DisabledReason;
};
