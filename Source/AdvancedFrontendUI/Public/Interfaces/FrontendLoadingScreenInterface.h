#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FrontendLoadingScreenInterface.generated.h"

// This interface is assigned in the player controller to have the interface events available
// it also gets checked in the loading screen subsystem
UINTERFACE(BlueprintType)
class UFrontendLoadingScreenInterface : public UInterface
{
	GENERATED_BODY()
};

class ADVANCEDFRONTENDUI_API IFrontendLoadingScreenInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent) void OnLoadingScreenActivated();
	UFUNCTION(BlueprintNativeEvent) void OnLoadingScreenDeactivated();
};
