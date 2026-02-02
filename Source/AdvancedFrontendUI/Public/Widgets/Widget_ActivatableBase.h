#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Widget_ActivatableBase.generated.h"

class AFrontendPlayerController;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class ADVANCEDFRONTENDUI_API UWidget_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintPure) AFrontendPlayerController* GetOwningFrontendPlayerController();
	
private:
	TWeakObjectPtr<AFrontendPlayerController> CachedOwningFrontendController;
};
