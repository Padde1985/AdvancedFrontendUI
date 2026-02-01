#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushSoftWidget.generated.h"

class UWidget_ActivatableBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetDelegate, UWidget_ActivatableBase*, PushedWidget);

UCLASS()
class ADVANCEDFRONTENDUI_API UAsyncAction_PushSoftWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable) FOnPushSoftWidgetDelegate OnWidgetCreatedBeforePush;
	UPROPERTY(BlueprintAssignable) FOnPushSoftWidgetDelegate OnWidgetCreatedAfterPush;
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Push Soft Widget To Widget Stack"	)) 
	static UAsyncAction_PushSoftWidget* PushSoftWidget(const UObject* WorldContextObject, APlayerController* PlayerController, 
	                                                   TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, FGameplayTag InWidgetStackTag, 
	                                                   bool bFocusOnNewlyPushedWidget = true);
	
	virtual void Activate() override;
	
private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	TWeakObjectPtr<APlayerController> CachedPlayerController;
	TSoftClassPtr<UWidget_ActivatableBase> CachedSoftWidgetClass;
	FGameplayTag CachedWidgetStackTag;
	bool bCachedFocusOnNewlyPushedWidget = false;
};
