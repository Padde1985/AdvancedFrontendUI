#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "FrontendDeveloperSettings.generated.h"

class UWidget_ActivatableBase;

UCLASS(Config = Game, DefaultConfig, meta=(DisplayName = "Frontend UI Settings"))
class ADVANCEDFRONTENDUI_API UFrontendDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category="Widget Reference", meta = (Categories = "Frontend.Widget")) TMap<FGameplayTag,TSoftClassPtr<UWidget_ActivatableBase>> FrontendWidgetMap;
};
