#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "FrontendUISubsystem.generated.h"

struct FGameplayTag;
class UWidget_ActivatableBase;
class UWidget_PrimaryLayout;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush,
};

UCLASS()
class ADVANCEDFRONTENDUI_API UFrontendUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	static UFrontendUISubsystem* Get(const UObject* WorldContextObject);
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	void PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStack, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushCallback);
	
	UFUNCTION(BlueprintCallable) void RegisterPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget);
	
private:
	UPROPERTY(Transient) UWidget_PrimaryLayout* PrimaryLayout;
};
