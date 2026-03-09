#include "Widgets/Widget_ActivatableBase.h"
#include "Controllers/FrontendPlayerController.h"

// get the current player controller
AFrontendPlayerController* UWidget_ActivatableBase::GetOwningFrontendPlayerController()
{
	if (!this->CachedOwningFrontendController.IsValid())
	{
		this->CachedOwningFrontendController = GetOwningPlayer<AFrontendPlayerController>();
	}
	
	return this->CachedOwningFrontendController.IsValid() ? this->CachedOwningFrontendController.Get() : nullptr; 
}
