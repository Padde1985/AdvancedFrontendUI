#include "AdvancedFrontendUI/Public/Controllers/FrontendPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Subsystems/FrontendUISubsystem.h"
#include "Widgets/Widget_PrimaryLayout.h"

// BeginPlayingState is called on servers and clients and is therefore better than OnPossess which is only called on servers
void AFrontendPlayerController::BeginPlayingState()
{
	if (!IsLocalController()) return;
	
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
