#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "FrontendGameUserSettings.generated.h"

// is set in project settings under GameUserSettings
UCLASS()
class ADVANCEDFRONTENDUI_API UFrontendGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	static UFrontendGameUserSettings* Get();

	UFUNCTION() FString GetCurrentGameDifficulty() const;
	UFUNCTION() void SetCurrentGameDifficulty(const FString& InNewDifficulty);
	
private:
	UPROPERTY(Config) FString CurrentGameDifficulty;
};
