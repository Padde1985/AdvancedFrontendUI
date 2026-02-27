#include "FrontendSettings/FrontendGameUserSettings.h"

#include "FrontendSettings/FrontendDeveloperSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

UFrontendGameUserSettings::UFrontendGameUserSettings() 
: OverallVolume(1.f), MusicVolume(1.f), SoundFXVolume(1.f), bAllowBackgroundAudio(false), bUseHDRAudio(false)
{
}

UFrontendGameUserSettings* UFrontendGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UFrontendGameUserSettings>(GEngine->GetGameUserSettings());
	}
	
	return nullptr;
}

FString UFrontendGameUserSettings::GetCurrentGameDifficulty() const
{
	return this->CurrentGameDifficulty;
}

void UFrontendGameUserSettings::SetCurrentGameDifficulty(const FString& InNewDifficulty)
{
	this->CurrentGameDifficulty = InNewDifficulty;
}

float UFrontendGameUserSettings::GetOverallVolume() const
{
	return this->OverallVolume;
}

void UFrontendGameUserSettings::SetOverallVolume(const float InOverallVolume)
{
	UWorld* InAudioWorld = nullptr;
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
	
	if (GEngine) InAudioWorld = GEngine->GetCurrentPlayWorld();
	
	if (!InAudioWorld || !FrontendDeveloperSettings) return;
	
	USoundClass* MasterSoundClass = nullptr;
	if (UObject* LoadedObject = FrontendDeveloperSettings->MasterSoundClass.TryLoad()) MasterSoundClass = CastChecked<USoundClass>(LoadedObject);
	
	USoundMix* DefaultSoundMix = nullptr;
	if (UObject* LoadedObject = FrontendDeveloperSettings->DefaultSoundMix.TryLoad()) DefaultSoundMix = CastChecked<USoundMix>(LoadedObject);
	
	this->OverallVolume = InOverallVolume;
	
	UGameplayStatics::SetSoundMixClassOverride(InAudioWorld, DefaultSoundMix, MasterSoundClass, this->OverallVolume, 1.f, 0.2f);
	UGameplayStatics::PushSoundMixModifier(InAudioWorld, DefaultSoundMix);
}

float UFrontendGameUserSettings::GetMusicVolume() const
{
	return this->MusicVolume;
}

void UFrontendGameUserSettings::SetMusicVolume(const float InMusicVolume)
{
	this->MusicVolume = InMusicVolume;
}

float UFrontendGameUserSettings::GetSoundFXVolume() const
{
	return this->SoundFXVolume;
}

void UFrontendGameUserSettings::SetSoundFXVolume(const float InSoundFXVolume)
{
	this->SoundFXVolume = InSoundFXVolume;
}

bool UFrontendGameUserSettings::GetAllowBackgroundAudio() const
{
	return this->bAllowBackgroundAudio;
}

void UFrontendGameUserSettings::SetAllowBackgroundAudio(const bool InAllowBackgroundAudio)
{
	this->bAllowBackgroundAudio = InAllowBackgroundAudio;
}

bool UFrontendGameUserSettings::GetUseHDRAudio() const
{
	return this->bUseHDRAudio;
}

void UFrontendGameUserSettings::SetUseHDRAudio(const bool InUseHDRAudio)
{
	this->bUseHDRAudio = InUseHDRAudio;
}

float UFrontendGameUserSettings::GetCurrentDisplayGamma() const
{
	if (GEngine) return GEngine->GetDisplayGamma();
	
	return 0.f;
}

void UFrontendGameUserSettings::SetCurrentDisplayGamma(const float InCurrentDisplayGamma)
{
	if (GEngine) GEngine->DisplayGamma = InCurrentDisplayGamma;
}
