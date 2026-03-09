#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "FrontendPlayerController.generated.h"

class UWidget_PrimaryLayout;

// Player Controller class, connects the user inputs with the game logic
UCLASS(PrioritizeCategories=("FrontendUI", "Input"))
class ADVANCEDFRONTENDUI_API AFrontendPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent) void BeginPlayingStateBP();
	
protected:
	UPROPERTY(EditAnywhere, Category = "FrontendUI") TSubclassOf<UUserWidget> PrimaryLayoutWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Input") TObjectPtr<UInputMappingContext> InputMappingContext;
	
	virtual void OnPossess(APawn* aPawn) override;

	UFUNCTION(BlueprintCallable) void InitializeWidget();
	
private:
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true)) bool bIsLoadFinsihed = false;
};
