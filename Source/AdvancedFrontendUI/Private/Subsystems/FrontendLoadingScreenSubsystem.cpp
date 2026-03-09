#include "Subsystems/FrontendLoadingScreenSubsystem.h"
#include "PreLoadScreenManager.h"
#include "Blueprint/UserWidget.h"
#include "FrontendSettings/FrontendLoadingScreenSettings.h"
#include "Interfaces/FrontendLoadingScreenInterface.h"

// interface function from the Game instance parent class and serves as a checkpoint before the subsystem is being created
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

// initialization
void UFrontendLoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::OnMapPreLoaded);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnMapPostLoaded);
}

// deinitalization, is called before the subsystem is being destroyed
void UFrontendLoadingScreenSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

// return the Owning World Object that is tickable (the World object could be different to the gameplay world)
UWorld* UFrontendLoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
	if (UGameInstance* OwningGameInstance = GetGameInstance()) return OwningGameInstance->GetWorld();
	
	return nullptr;
}

// Tick function as in all tickable objects
void UFrontendLoadingScreenSubsystem::Tick(float DeltaTime)
{
	this->TryUpdateLoadingScreen();
}

// returns the tickable type
// never = Tick never gets called
// always = Tick is called every frame with no further checks
// conditional = Tick is only called when function IsTickable returns true
ETickableTickType UFrontendLoadingScreenSubsystem::GetTickableTickType() const
{	
	if (IsTemplate()) return ETickableTickType::Never;
	
	// works together with IsTickable, if set to always, IsTickable is not called
	return ETickableTickType::Conditional;
}

// check if object should Tick
bool UFrontendLoadingScreenSubsystem::IsTickable() const
{	
	return GetGameInstance() && GetGameInstance()->GetGameViewportClient();
}

// benchmark function, has to be overridden by the child class when implementing the interface
TStatId UFrontendLoadingScreenSubsystem::GetStatId() const
{
	// returns benchmarks provided by Unreal Engine
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFrontendLoadingScreenSubsystem, STATGROUP_Tickables);
}

// gets called as callback function of the engine
void UFrontendLoadingScreenSubsystem::OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName)
{
	if (WorldContext.OwningGameInstance != GetGameInstance()) return;
	
	SetTickableTickType(ETickableTickType::Conditional);
	
	this->bIsCurrentlyLoadingMap = true;
	
	this->TryUpdateLoadingScreen();
}

// gets called as callback of the engine
void UFrontendLoadingScreenSubsystem::OnMapPostLoaded(UWorld* LoadedWorld)
{
	if (LoadedWorld && LoadedWorld->GetGameInstance() == GetGameInstance()) this->bIsCurrentlyLoadingMap = false;
}

// update the loading screen with the current loading reason or step
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

// checks if there is a pre-load screen active (defined in the unreal editor in Project settings)
bool UFrontendLoadingScreenSubsystem::IsPreLoadScreenActive() const
{
	if (FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get()) return PreLoadScreenManager->HasValidActivePreLoadScreen();
	
	return false;
}

// determine if the loading screen should be shown
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

// check if loading screen should be displayed (several areas have to be fully loaded to prevent it from being shown)
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

// create the widget if not existent yet
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

// remove the loading screen from viewport and show start menu instead
void UFrontendLoadingScreenSubsystem::TryRemoveLoadingScreen()
{
	if (!this->CachedCreatedLoadingScreenWidget) return;
	
	GetGameInstance()->GetGameViewportClient()->RemoveViewportWidgetContent(this->CachedCreatedLoadingScreenWidget.ToSharedRef());
	this->CachedCreatedLoadingScreenWidget.Reset();
}

// loading screen visbility changed (either it is just being displayed or was just destroyed)
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
