#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FrontendFunctionLibrary.generated.h"

class UWidget_ActivatableBase;

UCLASS()
class ADVANCEDFRONTENDUI_API UFrontendFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Frontend Function Library", meta=(Categories = "Frontend.Widget")) static TSoftClassPtr<UWidget_ActivatableBase> GetFrontendSoftWidgetClassByTag(FGameplayTag InWidgetTag);
};
