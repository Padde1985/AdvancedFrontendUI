#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widget_PrimaryLayout.generated.h"

class UCommonActivatableWidgetContainerBase;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class ADVANCEDFRONTENDUI_API UWidget_PrimaryLayout : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UCommonActivatableWidgetContainerBase* FindWidgetStackByTag(const FGameplayTag& InTag) const;
	
protected:
	UFUNCTION(BlueprintCallable, meta=(Categories = "Frontend.WidgetStack")) void RegisterWidgetStack(FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack);
	
private:
	UPROPERTY(Transient) TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> RegisteredWidgetStackMap; // transient initializes variable at runtime
};
