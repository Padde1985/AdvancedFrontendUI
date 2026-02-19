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
	UPROPERTY(Config, EditAnywhere, Category="Options Image Reference", meta = (Categories = "Frontend.Image")) TMap<FGameplayTag, TSoftObjectPtr<UTexture2D>> OptionsScreenSoftImageMap;
	UPROPERTY(Config, EditAnywhere, Category="Audio", meta=(AllowedClasses = "/Script/Engine.SoundClass")) FSoftObjectPath MasterSoundClass;
	UPROPERTY(Config, EditAnywhere, Category="Audio", meta=(AllowedClasses = "/Script/Engine.SoundClass")) FSoftObjectPath MusicSoundClass;
	UPROPERTY(Config, EditAnywhere, Category="Audio", meta=(AllowedClasses = "/Script/Engine.SoundClass")) FSoftObjectPath SoundFXSoundClass;
	UPROPERTY(Config, EditAnywhere, Category="Audio", meta=(AllowedClasses = "/Script/Engine.SoundMix")) FSoftObjectPath DefaultSoundMix;
};
