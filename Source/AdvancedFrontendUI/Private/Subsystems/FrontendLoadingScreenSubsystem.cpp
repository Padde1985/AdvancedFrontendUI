#include "Subsystems/FrontendLoadingScreenSubsystem.h"
#include "PreLoadScreenManager.h"
#include "Blueprint/UserWidget.h"
#include "FrontendSettings/FrontendLoadingScreenSettings.h"
#include "Interfaces/FrontendLoadingScreenInterface.h"

bool UFrontendLoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses; 
		GetDerivedClasses(GetClass(), FoundClasses);
		
		return FoundClasses.IsEmpty();
	}
	
	return false;
}

void UFrontendLoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::OnMapPreLoaded);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnMapPostLoaded);
}

void UFrontendLoadingScreenSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

UWorld* UFrontendLoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
	if (UGameInstance* OwningGameInstance = GetGameInstance()) return OwningGameInstance->GetWorld();
	
	return nullptr;
}

void UFrontendLoadingScreenSubsystem::Tick(float DeltaTime)
{
	this->TryUpdateLoadingScreen();
}

ETickableTickType UFrontendLoadingScreenSubsystem::GetTickableTickType() const
{	
	//if (IsTemplate()) return ETickableTickType::Never;
	
	// works together with IsTickable, if set to always, IsTickable is not called
	return ETickableTickType::Conditional;
}

bool UFrontendLoadingScreenSubsystem::IsTickable() const
{
	const bool bIsGameInstance = GetGameInstance() ? true : false;
	bool bGameViewportClient = false;
	if (bIsGameInstance) bGameViewportClient = GetGameInstance()->GetGameViewportClient() ? true : false;
	
	return bIsGameInstance && bGameViewportClient;
}

TStatId UFrontendLoadingScreenSubsystem::GetStatId() const
{
	// returns benchmarks provided by Unreal Engine
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFrontendLoadingScreenSubsystem, STATGROUP_Tickables);
}

void UFrontendLoadingScreenSubsystem::OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName)
{
	if (WorldContext.OwningGameInstance != GetGameInstance()) return;
	
	SetTickableTickType(ETickableTickType::Conditional);
	
	this->bIsCurrentlyLoadingMap = true;
	
	this->TryUpdateLoadingScreen();
}

void UFrontendLoadingScreenSubsystem::OnMapPostLoaded(UWorld* LoadedWorld)
{
	if (LoadedWorld && LoadedWorld->GetGameInstance() == GetGameInstance()) this->bIsCurrentlyLoadingMap = false;
}

void UFrontendLoadingScreenSubsystem::TryUpdateLoadingScreen()
{
	if (this->IsPreLoadScreenActive()) return;
	
	if (this->ShouldShowLoadingScreen())
	{
		this->TryDisplayLoadingScreenIfNone();
		
		this->OnLoadingReasonUpdated.Broadcast(this->CurrentLoadingReason);
	}
	else
	{
		this->TryRemoveLoadingScreen();
		this->HoldLoadingScreenStartUpTime = -1.f;
		
		this->NotifyLoadingScreenVisibilityChanged(false);
		
		SetTickableTickType(ETickableTickType::Never);	
	}
}

bool UFrontendLoadingScreenSubsystem::IsPreLoadScreenActive() const
{
	if (FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get()) return PreLoadScreenManager->HasValidActivePreLoadScreen();
	
	return false;
}

bool UFrontendLoadingScreenSubsystem::ShouldShowLoadingScreen()
{
	const UFrontendLoadingScreenSettings* LoadingScreenSettings = GetDefault<UFrontendLoadingScreenSettings>();
	if (GIsEditor && !LoadingScreenSettings->bShowLoadingScreenInEditor) return false;
	
	if (this->CheckTheNeedToShowLoadingScreen())
	{
		GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = true;
		
		return true;
	}
	
	this->CurrentLoadingReason = TEXT("Waiting for Texture streaming");
	
	GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = false;
	
	const float CurrentTime = FPlatformTime::Seconds();
	
	if (this->HoldLoadingScreenStartUpTime < 0.f) this->HoldLoadingScreenStartUpTime = CurrentTime;
	
	const float ElapsedTime = CurrentTime - this->HoldLoadingScreenStartUpTime;
	if (ElapsedTime < LoadingScreenSettings->HoldLoadingScreenExtraSeconds) return true;
	
	return false;
}

bool UFrontendLoadingScreenSubsystem::CheckTheNeedToShowLoadingScreen()
{
	if (this->bIsCurrentlyLoadingMap)
	{
		this->CurrentLoadingReason = TEXT("Loading Level");
		
		return true;
	}
	
	UWorld* OwningWorld = GetGameInstance()->GetWorld();
	if (!OwningWorld)
	{
		this->CurrentLoadingReason = TEXT("Initializing World");
		
		return true;
	}
	
	if (!OwningWorld->HasBegunPlay())
	{
		this->CurrentLoadingReason = TEXT("World hasn't begun play yet");
		
		return true;
	}
	
	if (!OwningWorld->GetFirstPlayerController())
	{
		this->CurrentLoadingReason = TEXT("World hasn't loaded first player controller");
		
		return true;
	}
	
	return false;
}

void UFrontendLoadingScreenSubsystem::TryDisplayLoadingScreenIfNone()
{
	if (this->CachedCreatedLoadingScreenWidget) return;
	
	const UFrontendLoadingScreenSettings* LoadingScreenSettings = GetDefault<UFrontendLoadingScreenSettings>();
	TSubclassOf<UUserWidget> LoadedWidgetClass = LoadingScreenSettings->GetLoadingScreenWidgetClassChecked();
	
	UUserWidget* CreatedWidget = UUserWidget::CreateWidgetInstance(*GetGameInstance(), LoadedWidgetClass, NAME_None);
	
	check(CreatedWidget);
	
	this->CachedCreatedLoadingScreenWidget = CreatedWidget->TakeWidget();
	GetGameInstance()->GetGameViewportClient()->AddViewportWidgetContent(this->CachedCreatedLoadingScreenWidget.ToSharedRef(), 1000);
	
	this->NotifyLoadingScreenVisibilityChanged(true);
}

void UFrontendLoadingScreenSubsystem::TryRemoveLoadingScreen()
{
	if (!this->CachedCreatedLoadingScreenWidget) return;
	
	GetGameInstance()->GetGameViewportClient()->RemoveViewportWidgetContent(this->CachedCreatedLoadingScreenWidget.ToSharedRef());
	this->CachedCreatedLoadingScreenWidget.Reset();
}

void UFrontendLoadingScreenSubsystem::NotifyLoadingScreenVisibilityChanged(bool bIsVisible)
{
	for (ULocalPlayer* ExistingLocalPlayer : GetGameInstance()->GetLocalPlayers())
	{
		if (!ExistingLocalPlayer) continue;
		
		if (APlayerController* PC = ExistingLocalPlayer->GetPlayerController(GetGameInstance()->GetWorld()))
		{
			if (PC->Implements<UFrontendLoadingScreenInterface>())
			{
				if (bIsVisible)
				{
					IFrontendLoadingScreenInterface::Execute_OnLoadingScreenActivated(PC);
				}
				else
				{
					IFrontendLoadingScreenInterface::Execute_OnLoadingScreenDeactivated(PC);
				}
			}
			
			if (APawn* OwningPawn = PC->GetPawn())
			{
				if (OwningPawn->Implements<UFrontendLoadingScreenInterface>())
				{
					if (bIsVisible)
					{
						IFrontendLoadingScreenInterface::Execute_OnLoadingScreenActivated(OwningPawn);
					}
					else
					{
						IFrontendLoadingScreenInterface::Execute_OnLoadingScreenDeactivated(OwningPawn);
					}
				}
			}
		}
		
		// notify other objects in the world
	}
}
