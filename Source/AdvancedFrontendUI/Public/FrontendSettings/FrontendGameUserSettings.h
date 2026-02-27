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
	UFrontendGameUserSettings();
	
	static UFrontendGameUserSettings* Get();

	UFUNCTION() FString GetCurrentGameDifficulty() const;
	UFUNCTION() void SetCurrentGameDifficulty(const FString& InNewDifficulty);
	UFUNCTION() float GetOverallVolume() const;
	UFUNCTION() void SetOverallVolume(const float InOverallVolume);
	UFUNCTION() float GetMusicVolume() const;
	UFUNCTION() void SetMusicVolume(const float InMusicVolume);
	UFUNCTION() float GetSoundFXVolume() const;
	UFUNCTION() void SetSoundFXVolume(const float InSoundFXVolume);
	UFUNCTION() bool GetAllowBackgroundAudio() const;
	UFUNCTION() void SetAllowBackgroundAudio(const bool InAllowBackgroundAudio);
	UFUNCTION() bool GetUseHDRAudio() const;
	UFUNCTION() void SetUseHDRAudio(const bool InUseHDRAudio);
	UFUNCTION() float GetCurrentDisplayGamma() const;
	UFUNCTION() void SetCurrentDisplayGamma(const float InCurrentDisplayGamma);
	
private:
	UPROPERTY(Config) FString CurrentGameDifficulty;
	UPROPERTY(Config) float OverallVolume;
	UPROPERTY(Config) float MusicVolume;
	UPROPERTY(Config) float SoundFXVolume;
	UPROPERTY(Config) bool bAllowBackgroundAudio;
	UPROPERTY(Config) bool bUseHDRAudio;
};
