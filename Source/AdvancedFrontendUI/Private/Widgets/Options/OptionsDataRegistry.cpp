#include "Widgets/Options/OptionsDataRegistry.h"

#include "FrontendFunctionLibrary.h"
#include "FrontendGameplayTags.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringBool.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterFuncName))

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	this->InitGameplayCollectionTab();
	this->InitAudioCollectionTab();
	this->InitVideoCollectionTab();
	this->InitControllsCollectionTab();
}

const TArray<UListDataObject_Collection*>& UOptionsDataRegistry::GetRegisteredOptionsTabCollection() const
{
	return this->RegisteredOptionsTabCollection;
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const
{
	UListDataObject_Collection* const* FoundTabCollectionPtr = this->RegisteredOptionsTabCollection.FindByPredicate(
		[InSelectedTabID](const UListDataObject_Collection* AvailableTabCollection)
	{
		return AvailableTabCollection->GetDataID() == InSelectedTabID;
	});
	
	checkf(FoundTabCollectionPtr, TEXT("No valid tab found under the ID %s"), *InSelectedTabID.ToString());
	
	UListDataObject_Collection* FoundTabCollection = *FoundTabCollectionPtr;
	
	TArray<UListDataObject_Base*> AllChildListItems;
	
	for (UListDataObject_Base* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		if (!ChildListData) continue;
		
		AllChildListItems.Add(ChildListData);
		
		if (ChildListData->HasAnyChildListData()) this->FindChildListDataRecursively(ChildListData, AllChildListItems);
	}
	
	return AllChildListItems;
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObject_Collection* GameplayTabCollection = NewObject<UListDataObject_Collection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString(TEXT("Gameplay")));
	
	// this is the full code for constructing the data interaction helper. We use the macro MAKE_OPTIONS_DATA_CONTROL instead
	//TSharedPtr<FOptionsDataInteractionHelper> ConstructedHelper = MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, GetCurrentGameDifficulty));
	
	UListDataObject_String* GameDifficulty = NewObject<UListDataObject_String>();
	GameDifficulty->SetDataID(FName("GameDifficulty"));
	GameDifficulty->SetDataDisplayName(FText::FromString(TEXT("Difficulty")));
	GameDifficulty->SetDescriptionRichText(FText::FromString(
	TEXT("Adjust the difficulty of the game experience.\n\n"
		 "<Bold>Easy:</> Focuses on the story experience. Provides the most relaxing combat.\n\n"
		 "<Bold>Normal:</> Offers slightly other combat experience.\n\n"
		 "<Bold>Hard:</> Offers a much more challenging combat experience.\n\n"
		 "<Bold>Very Hard:</> Provides the most challenging combat experience. Not recommended for first play through")
		 ));
	GameDifficulty->AddDynamicOption(TEXT("Easy"), FText::FromString(TEXT("Easy")));
	GameDifficulty->AddDynamicOption(TEXT("Normal"), FText::FromString(TEXT("Normal")));
	GameDifficulty->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("Hard")));
	GameDifficulty->AddDynamicOption(TEXT("Very Hard"), FText::FromString(TEXT("Very Hard")));
	GameDifficulty->SetDefaultValueFromString(TEXT("Normal"));
	GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
	GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
	GameDifficulty->SetShouldApplaySettingsImmediately(true);
	
	GameplayTabCollection->AddChildListData(GameDifficulty);
	
	UListDataObject_String* ImageItem = NewObject<UListDataObject_String>();
	ImageItem->SetDataID(FName("ImageItem"));
	ImageItem->SetDataDisplayName(FText::FromString(TEXT("Image Item")));
	ImageItem->SetSoftDescriptionImage(UFrontendFunctionLibrary::GetOptionsSoftImageByTag(FrontendGameplayTags::Frontend_Image_TestImage));
	ImageItem->SetDescriptionRichText(FText::FromString(TEXT("The image to be displayed can be set in the Project settings")));
	
	GameplayTabCollection->AddChildListData(ImageItem);
	
	this->RegisteredOptionsTabCollection.Add(GameplayTabCollection);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	// create the Audio tab itself
	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString(TEXT("Audio")));
	
	// create sub category Volume
	UListDataObject_Collection* VolumeCategoryCollection = NewObject<UListDataObject_Collection>();
	VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
	VolumeCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Volume")));
	
	// add element for overall sound volume
	UListDataObject_Scalar* OverallVolume = NewObject<UListDataObject_Scalar>();
	OverallVolume->SetDataID(FName("OverallVolume"));
	OverallVolume->SetDataDisplayName(FText::FromString(TEXT("Overall Volume")));
	OverallVolume->SetDescriptionRichText(FText::FromString(TEXT("This is a description for the overall volume")));
	OverallVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f)); // 0 to 100%
	OverallVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
	OverallVolume->SetSliderStepSize(0.01f);
	OverallVolume->SetDefaultValueFromString(LexToString(1.f));
	OverallVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
	OverallVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
	OverallVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallVolume));
	OverallVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallVolume));
	OverallVolume->SetShouldApplaySettingsImmediately(true);
	
	VolumeCategoryCollection->AddChildListData(OverallVolume);
	
	// add element for music volume
	UListDataObject_Scalar* MusicVolume = NewObject<UListDataObject_Scalar>();
	MusicVolume->SetDataID(FName("MusicVolume"));
	MusicVolume->SetDataDisplayName(FText::FromString(TEXT("Music Volume")));
	MusicVolume->SetDescriptionRichText(FText::FromString(TEXT("This is a description for the music volume")));
	MusicVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f)); // 0 to 100%
	MusicVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
	MusicVolume->SetSliderStepSize(0.01f);
	MusicVolume->SetDefaultValueFromString(LexToString(1.f));
	MusicVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
	MusicVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
	MusicVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMusicVolume));
	MusicVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMusicVolume));
	MusicVolume->SetShouldApplaySettingsImmediately(true);
	
	VolumeCategoryCollection->AddChildListData(MusicVolume);
	
	// add element for sound effects volume
	UListDataObject_Scalar* SoundFXVolume = NewObject<UListDataObject_Scalar>();
	SoundFXVolume->SetDataID(FName("SoundFXVolume"));
	SoundFXVolume->SetDataDisplayName(FText::FromString(TEXT("SoundFX Volume")));
	SoundFXVolume->SetDescriptionRichText(FText::FromString(TEXT("This is a description for the SoundFX volume")));
	SoundFXVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f)); // 0 to 100%
	SoundFXVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
	SoundFXVolume->SetSliderStepSize(0.01f);
	SoundFXVolume->SetDefaultValueFromString(LexToString(1.f));
	SoundFXVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
	SoundFXVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
	SoundFXVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetSoundFXVolume));
	SoundFXVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetSoundFXVolume));
	SoundFXVolume->SetShouldApplaySettingsImmediately(true);
	
	VolumeCategoryCollection->AddChildListData(SoundFXVolume);
	
	// add entire category to the Audio tab
	AudioTabCollection->AddChildListData(VolumeCategoryCollection);
	
	// create sub category for Sound settings
	UListDataObject_Collection* SoundCategoryCollection = NewObject<UListDataObject_Collection>();
	SoundCategoryCollection->SetDataID(FName("SoundCategoryCollection"));
	SoundCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Sound")));
	
	// add element for running sounds in background mode (not focused, or minimized)
	UListDataObject_StringBool* AllowBackgroundAudio = NewObject<UListDataObject_StringBool>();
	AllowBackgroundAudio->SetDataID(FName("AllowBackgroundAudio"));
	AllowBackgroundAudio->SetDataDisplayName(FText::FromString(TEXT("Allow Background Audio")));
	AllowBackgroundAudio->OverrideTrueDisplayText(FText::FromString(TEXT("Enabled")));
	AllowBackgroundAudio->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
	AllowBackgroundAudio->SetFalseAsDefaultValue();
	AllowBackgroundAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowBackgroundAudio));
	AllowBackgroundAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowBackgroundAudio));
	AllowBackgroundAudio->SetShouldApplaySettingsImmediately(true);
	
	SoundCategoryCollection->AddChildListData(AllowBackgroundAudio);
	
	// add element for playing HDR sound
	UListDataObject_StringBool* UseHDRAudio = NewObject<UListDataObject_StringBool>();
	UseHDRAudio->SetDataID(FName("UseHDRAudio"));
	UseHDRAudio->SetDataDisplayName(FText::FromString(TEXT("Use HDR Audio")));
	UseHDRAudio->OverrideTrueDisplayText(FText::FromString(TEXT("Enabled")));
	UseHDRAudio->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
	UseHDRAudio->SetFalseAsDefaultValue();
	UseHDRAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetUseHDRAudio));
	UseHDRAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetUseHDRAudio));
	UseHDRAudio->SetShouldApplaySettingsImmediately(true);
	
	SoundCategoryCollection->AddChildListData(UseHDRAudio);
	
	// add entire catgeory to Audio tab
	AudioTabCollection->AddChildListData(SoundCategoryCollection);
	
	// add the audio tab itself to the tab collection
	this->RegisteredOptionsTabCollection.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString(TEXT("Video")));
	
	this->RegisteredOptionsTabCollection.Add(VideoTabCollection);
}

void UOptionsDataRegistry::InitControllsCollectionTab()
{
	UListDataObject_Collection* ControllsTabCollection = NewObject<UListDataObject_Collection>();
	ControllsTabCollection->SetDataID(FName("ControllsTabCollection"));
	ControllsTabCollection->SetDataDisplayName(FText::FromString(TEXT("Controlls")));
	
	this->RegisteredOptionsTabCollection.Add(ControllsTabCollection);
}

void UOptionsDataRegistry::FindChildListDataRecursively(UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutFoundChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData()) return;
	
	for (UListDataObject_Base* Child : InParentData->GetAllChildListData())
	{
		if (!Child)	continue;
 
		OutFoundChildListData.Add(Child);
 
		if (Child->HasAnyChildListData())
		{
			this->FindChildListDataRecursively(Child, OutFoundChildListData);
		}
	}
}
