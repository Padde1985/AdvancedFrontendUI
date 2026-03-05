#include "AdvancedFrontendUI/Public/Controllers/FrontendPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Subsystems/FrontendUISubsystem.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Widgets/Widget_PrimaryLayout.h"

// BeginPlayingState is called on servers and clients and is therefore better than OnPossess which is only called on servers
void AFrontendPlayerController::BeginPlayingState()
{
	if (!IsLocalController()) return;
	
	if (this->bIsLoadFinsihed) this->InitializeWidget();
	
	UFrontendGameUserSettings* GameUserSettings = UFrontendGameUserSettings::Get();
	if (GameUserSettings->GetLastCPUBenchmarkResult() == -1.f || GameUserSettings->GetLastGPUBenchmarkResult() == -1.f)
	{
		GameUserSettings->RunHardwareBenchmark();
		GameUserSettings->ApplyHardwareBenchmarkResults();
	}
	
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = Cast<ULocalPlayer>(Player)->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		// Register input context does not activate the actions yet (as we are still in the menu) but gives us the key bindings and options to change them
		if (this->InputMappingContext) InputSubsystem->GetUserSettings()->RegisterInputMappingContext(this->InputMappingContext);
	}
}

void AFrontendPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	
	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName("Default"), FoundCameras);
	if (!FoundCameras.IsEmpty())
	{
		SetViewTarget(FoundCameras[0]);
	}
}

void AFrontendPlayerController::InitializeWidget()
{	
	if (UWidget_PrimaryLayout* PrimaryLayoutWidget = CreateWidget<UWidget_PrimaryLayout>(this, this->PrimaryLayoutWidgetClass))
	{
		PrimaryLayoutWidget->AddToViewport();
		
		if (UFrontendUISubsystem* Subsystem = UFrontendUISubsystem::Get(this))
		{
			Subsystem->RegisterPrimaryLayoutWidget(PrimaryLayoutWidget);
		}
	}
	// calls the Blueprint event
	this->BeginPlayingStateBP();
}
