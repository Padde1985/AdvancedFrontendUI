#include "FrontendSettings/FrontendGameUserSettings.h"
#include "FrontendSettings/FrontendDeveloperSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

//variable initialization
UFrontendGameUserSettings::UFrontendGameUserSettings() 
: OverallVolume(1.f), MusicVolume(1.f), SoundFXVolume(1.f), bAllowBackgroundAudio(false), bUseHDRAudio(false)
{
}

// get the game user settings from the engine
UFrontendGameUserSettings* UFrontendGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UFrontendGameUserSettings>(GEngine->GetGameUserSettings());
	}
	
	return nullptr;
}

// getter for game difficulty
FString UFrontendGameUserSettings::GetCurrentGameDifficulty() const
{
	return this->CurrentGameDifficulty;
}

// set game difficulty to the specified setting
void UFrontendGameUserSettings::SetCurrentGameDifficulty(const FString& InNewDifficulty)
{
	this->CurrentGameDifficulty = InNewDifficulty;
}

// getter for overall volume
float UFrontendGameUserSettings::GetOverallVolume() const
{
	return this->OverallVolume;
}

// set overall volume to the specified setting
void UFrontendGameUserSettings::SetOverallVolume(const float InOverallVolume)
{
	UWorld* InAudioWorld = nullptr;
	// get frontend developer sewttings (category in Project settings)
	// here are the settings for Music and sound asset
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
	
	if (GEngine) InAudioWorld = GEngine->GetCurrentPlayWorld();
	
	if (!InAudioWorld || !FrontendDeveloperSettings) return;
	
	// master sound class and sound mix are assets created for this project
	USoundClass* MasterSoundClass = nullptr;
	if (UObject* LoadedObject = FrontendDeveloperSettings->MasterSoundClass.TryLoad()) MasterSoundClass = CastChecked<USoundClass>(LoadedObject);
	
	USoundMix* DefaultSoundMix = nullptr;
	if (UObject* LoadedObject = FrontendDeveloperSettings->DefaultSoundMix.TryLoad()) DefaultSoundMix = CastChecked<USoundMix>(LoadedObject);
	
	this->OverallVolume = InOverallVolume;
	
	UGameplayStatics::SetSoundMixClassOverride(InAudioWorld, DefaultSoundMix, MasterSoundClass, this->OverallVolume, 1.f, 0.2f);
	UGameplayStatics::PushSoundMixModifier(InAudioWorld, DefaultSoundMix);
}

// getter for music volume
float UFrontendGameUserSettings::GetMusicVolume() const
{
	return this->MusicVolume;
}

// set music volume to the specified setting
void UFrontendGameUserSettings::SetMusicVolume(const float InMusicVolume)
{
	this->MusicVolume = InMusicVolume;
}

// getter for sound effects volume
float UFrontendGameUserSettings::GetSoundFXVolume() const
{
	return this->SoundFXVolume;
}

// set sound effects volume to the specified setting
void UFrontendGameUserSettings::SetSoundFXVolume(const float InSoundFXVolume)
{
	this->SoundFXVolume = InSoundFXVolume;
}

// getter for background audio flag
bool UFrontendGameUserSettings::GetAllowBackgroundAudio() const
{
	return this->bAllowBackgroundAudio;
}

// set background audio flag to the specified setting
void UFrontendGameUserSettings::SetAllowBackgroundAudio(const bool InAllowBackgroundAudio)
{
	this->bAllowBackgroundAudio = InAllowBackgroundAudio;
}

// getter for HDR audio
bool UFrontendGameUserSettings::GetUseHDRAudio() const
{
	return this->bUseHDRAudio;
}

// set HDR audio to the specified setting
void UFrontendGameUserSettings::SetUseHDRAudio(const bool InUseHDRAudio)
{
	this->bUseHDRAudio = InUseHDRAudio;
}

// getter for brightness
float UFrontendGameUserSettings::GetCurrentDisplayGamma() const
{
	if (GEngine) return GEngine->GetDisplayGamma();
	
	return 0.f;
}

// set brightness to the specified setting
void UFrontendGameUserSettings::SetCurrentDisplayGamma(const float InCurrentDisplayGamma)
{
	if (GEngine) GEngine->DisplayGamma = InCurrentDisplayGamma;
}