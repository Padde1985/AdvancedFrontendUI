#include "AsyncActions/AsyncAction_PushSoftWidget.h"
#include "Subsystems/FrontendUISubsystem.h"

UAsyncAction_PushSoftWidget* UAsyncAction_PushSoftWidget::PushSoftWidget(const UObject* WorldContextObject, APlayerController* PlayerController, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, FGameplayTag InWidgetStackTag, bool bFocusOnNewlyPushedWidget)
{
	checkf(!InSoftWidgetClass.IsNull(), TEXT("PushSoftWidgetToStack was passed a null soft widget class"));
	
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushSoftWidget* Node = NewObject<UAsyncAction_PushSoftWidget>();
			Node->CachedOwningWorld = World;
			Node->CachedPlayerController = PlayerController;
			Node->CachedSoftWidgetClass = InSoftWidgetClass;
			Node->CachedWidgetStackTag = InWidgetStackTag;
			Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;
			
			Node->RegisterWithGameInstance(World);
			
			return Node;
		}
	}
	
	return nullptr;
}

void UAsyncAction_PushSoftWidget::Activate()
{
	UFrontendUISubsystem* FrontendSubsystem = UFrontendUISubsystem::Get(this->CachedOwningWorld.Get());
	FrontendSubsystem->PushSoftWidgetToStackAsync(this->CachedWidgetStackTag, this->CachedSoftWidgetClass, 
							   [this](EAsyncPushWidgetState InPushState, UWidget_ActivatableBase* PushedWidget)
	{
		switch (InPushState)
		{
		case EAsyncPushWidgetState::OnCreatedBeforePush:
			PushedWidget->SetOwningPlayer(this->CachedPlayerController.Get());
			this->OnWidgetCreatedBeforePush.Broadcast(PushedWidget);
			break;
		case EAsyncPushWidgetState::AfterPush:
			this->OnWidgetCreatedAfterPush.Broadcast(PushedWidget);
			if (this->bCachedFocusOnNewlyPushedWidget)
			{
				if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusTarget())
				{
					WidgetToFocus->SetFocus();
				}
			}
			SetReadyToDestroy();
			break;
		default:
			break;
		}
	});
}
