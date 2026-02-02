#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "FrontendUISubsystem.generated.h"

class UFrontendCommonButtonBase;
struct FGameplayTag;
class UWidget_ActivatableBase;
class UWidget_PrimaryLayout;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionTextUpdatedDelegate, UFrontendCommonButtonBase*, BroadcastingButton, FText, DescriptionText);

UCLASS()
class ADVANCEDFRONTENDUI_API UFrontendUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable) FOnButtonDescriptionTextUpdatedDelegate OnButtonDescriptionTextUpdated;
	
	static UFrontendUISubsystem* Get(const UObject* WorldContextObject);
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	void PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStack, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushCallback);
	void PushConfirmScreenToModalStackAsync(EConfirmScreenType InScreenType, const FText& InScreenTitle, const FText& InScreenMessage, TFunction<void(EConfirmScreenButtonType)> ButtonClickedCallback);
	
	UFUNCTION(BlueprintCallable) void RegisterPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget);
	
private:
	UPROPERTY(Transient) UWidget_PrimaryLayout* PrimaryLayout;
};
