#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushConfirmScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmScreenButtonClickedEvent, EConfirmScreenButtonType, ClickedButtonType);

UCLASS()
class ADVANCEDFRONTENDUI_API UAsyncAction_PushConfirmScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable) FOnConfirmScreenButtonClickedEvent OnButtonClicked;
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Show Confirmation Screen"	))
	static UAsyncAction_PushConfirmScreen* PushConfirmScreen(const UObject* WorldContextObject, EConfirmScreenType ScreenType, FText InScreenTitle, FText InScreenMessage);
	
	virtual void Activate() override;
	
private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	EConfirmScreenType CachedConfirmScreenType;
	FText CachedScreenTitle;
	FText CachedScreenMessage;
};
