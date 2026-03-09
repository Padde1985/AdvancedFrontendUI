#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FrontendLoadingScreenSettings.generated.h"

class UUserWidget;

// creates a new category under project settings, the name is derived from teh class name
UCLASS(Config = Game, DefaultConfig)
class ADVANCEDFRONTENDUI_API UFrontendLoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings") TSoftClassPtr<UUserWidget> SoftLoadingScreenWidgetClass;
	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings") float HoldLoadingScreenExtraSeconds = 3.f;
	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings") bool bShowLoadingScreenInEditor = false;
	
	TSubclassOf<UUserWidget> GetLoadingScreenWidgetClassChecked() const;
};
