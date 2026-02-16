#include "FrontendSettings/FrontendGameUserSettings.h"

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
