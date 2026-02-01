#include "Subsystems/FrontendUISubsystem.h"
#include "Engine/AssetManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widgets/Widget_PrimaryLayout.h"

UFrontendUISubsystem* UFrontendUISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		
		return UGameInstance::GetSubsystem<UFrontendUISubsystem>(World->GetGameInstance());
	}
	
	return nullptr;
}

bool UFrontendUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses; 
		GetDerivedClasses(GetClass(), FoundClasses);
		
		return FoundClasses.IsEmpty();
	}
	
	return false;
}

void UFrontendUISubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStack, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushCallback)
{
	check(!InSoftWidgetClass.IsNull());
	
	// make sure to hand over the needed parameters one by one, in Async calls handing them over by & causes nullptr when accessing pointers
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(InSoftWidgetClass.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this, InSoftWidgetClass, InWidgetStack, AsyncPushCallback]()
	{
		UClass* LoadedWidgetClass = InSoftWidgetClass.Get();
		check(LoadedWidgetClass && this->PrimaryLayout);
		
		UCommonActivatableWidgetContainerBase* FoundWidgetStack = this->PrimaryLayout->FindWidgetStackByTag(InWidgetStack);
		UWidget_ActivatableBase* CreatedWidget = FoundWidgetStack->AddWidget<UWidget_ActivatableBase>(LoadedWidgetClass, [AsyncPushCallback](UWidget_ActivatableBase& CreatedWidgetInstance)
		{
			AsyncPushCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
		});
		AsyncPushCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
	}));
}

void UFrontendUISubsystem::RegisterPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget)
{
	check(InCreatedWidget);
	
	this->PrimaryLayout = InCreatedWidget;
}
