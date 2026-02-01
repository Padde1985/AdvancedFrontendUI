#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FrontendPlayerController.generated.h"

class UWidget_PrimaryLayout;

UCLASS(PrioritizeCategories=("FrontendUI"))
class ADVANCEDFRONTENDUI_API AFrontendPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent) void BeginPlayingStateBP();
	
protected:
	UPROPERTY(EditAnywhere, Category = "FrontendUI") TSubclassOf<UUserWidget> PrimaryLayoutWidgetClass;
	
	virtual void OnPossess(APawn* aPawn) override;
};
