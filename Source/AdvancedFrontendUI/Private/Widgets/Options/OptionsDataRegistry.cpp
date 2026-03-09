#include "Widgets/Options/OptionsDataRegistry.h"
#include "EnhancedInputSubsystems.h"
#include "FrontendFunctionLibrary.h"
#include "FrontendGameplayTags.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Internationalization/StringTableRegistry.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringBool.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringEnum.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringInteger.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"

// helper macro for dynamic setter and getter
#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterFuncName))

// helper macro for reading texts from a string table. The first parameter is the object path to the string table asset
// for a packaged build, under settings the directory for the string tables has to be set in additional asset to cook: Project Settings / Packaging / Advanced / Additional Asset Directories to Cook
#define GET_DESCRIPTION(InKey) \
	LOCTABLE("/Game/UI/StringTables/ST_OptionsScreenDescription.ST_OptionsScreenDescription", InKey)

// create the different tabs
void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	this->InitGameplayCollectionTab();
	this->InitAudioCollectionTab();
	this->InitVideoCollectionTab();
	this->InitControllsCollectionTab(InOwningLocalPlayer);
}

// return the tabs
const TArray<UListDataObject_Collection*>& UOptionsDataRegistry::GetRegisteredOptionsTabCollection() const
{
	return this->RegisteredOptionsTabCollection;
}

// get the tab content of the selected tab
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

// build the content for the gameplay tab
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

// build the content of the audio tab
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

// build the content for the video tab
void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString(TEXT("Video")));
	
	UListDataObject_StringEnum* CreatedWindowMode = nullptr;
	
	// Display Catgeory
	UListDataObject_Collection* DisplayCategoryCollection = NewObject<UListDataObject_Collection>();
	DisplayCategoryCollection->SetDataID(FName("DisplayCategoryCollection"));
	DisplayCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Display")));
	
	FOptionsDataEditConditionDescriptor PackagedBuildOnlyCondition;
	PackagedBuildOnlyCondition.SetEditConditionFunc([]()
	{
		const bool bIsInEditor = GIsEditor || GIsPlayInEditorWorld;
		
		return !bIsInEditor;
	});
	PackagedBuildOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This setting can only be adjusted in a packaged build</>"));
	
	//Window Mode
	UListDataObject_StringEnum* WindowMode = NewObject<UListDataObject_StringEnum>();
	WindowMode->SetDataID(FName("WindowMode"));
	WindowMode->SetDataDisplayName(FText::FromString(TEXT("Window Mode")));
	WindowMode->SetDescriptionRichText(GET_DESCRIPTION("WindowModeDescKey"));
	WindowMode->AddEnumOptions(EWindowMode::Fullscreen, FText::FromString(TEXT("Fullscreen")));
	WindowMode->AddEnumOptions(EWindowMode::WindowedFullscreen, FText::FromString(TEXT("Borderless Window")));
	WindowMode->AddEnumOptions(EWindowMode::Windowed, FText::FromString(TEXT("Windowed")));
	WindowMode->SetDefaultValueFromEnumOption(EWindowMode::WindowedFullscreen);
	WindowMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
	WindowMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));
	WindowMode->SetShouldApplaySettingsImmediately(true);
	WindowMode->AddEditCondition(PackagedBuildOnlyCondition);
	
	CreatedWindowMode = WindowMode;
	
	DisplayCategoryCollection->AddChildListData(WindowMode);
	
	// Screen resolution
	UListDataObject_StringResolution* ScreenResolution = NewObject<UListDataObject_StringResolution>();
	ScreenResolution->SetDataID(FName("ScreenResolution"));
	ScreenResolution->SetDataDisplayName(FText::FromString(TEXT("Screen Resolution")));
	ScreenResolution->SetDescriptionRichText(GET_DESCRIPTION("ScreenResolutionsDescKey"));
	ScreenResolution->InitResolutionValues();
	ScreenResolution->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
	ScreenResolution->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
	ScreenResolution->SetShouldApplaySettingsImmediately(true);
	ScreenResolution->AddEditCondition(PackagedBuildOnlyCondition);
	ScreenResolution->SetDefaultValueFromString(ScreenResolution->GetMaximumAllowedResolution());
	
	FOptionsDataEditConditionDescriptor WindowModeEditCondition;
	WindowModeEditCondition.SetEditConditionFunc([CreatedWindowMode]()
	{
		const bool bIsBorderlessWindow = CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::WindowedFullscreen;
		
		return !bIsBorderlessWindow;
	});
	WindowModeEditCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>Screen resolution is not adjustable when window mode is set to borderless window</>"));
	WindowModeEditCondition.SetDisabledForcedStringValue(ScreenResolution->GetMaximumAllowedResolution());
	
	ScreenResolution->AddEditCondition(WindowModeEditCondition);
	ScreenResolution->AddEditDependencyData(CreatedWindowMode);
	
	DisplayCategoryCollection->AddChildListData(ScreenResolution);	
	
	VideoTabCollection->AddChildListData(DisplayCategoryCollection);
	
	// Graphics category
	UListDataObject_Collection* GraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
	GraphicsCategoryCollection->SetDataID(FName("GraphicsCategoryCollection"));
	GraphicsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Graphics")));
	
	// Brightness
	UListDataObject_Scalar* DisplayGamma = NewObject<UListDataObject_Scalar>();
	DisplayGamma->SetDataID(FName("DisplayGamma"));
	DisplayGamma->SetDataDisplayName(FText::FromString(TEXT("Gamma")));
	DisplayGamma->SetDescriptionRichText(GET_DESCRIPTION("DisplayGammaDescKey"));
	DisplayGamma->SetDisplayValueRange(TRange<float>(0.f, 1.f));
	DisplayGamma->SetOutputValueRange(TRange<float>(0.5f, 3.9f)); // Unreal default value is 2.2f, which is 50% (0.5f) of the Display range
	DisplayGamma->SetSliderStepSize(0.01f);
	DisplayGamma->SetDisplayNumericType(ECommonNumericType::Percentage);
	DisplayGamma->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
	DisplayGamma->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentDisplayGamma));
	DisplayGamma->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentDisplayGamma));
	DisplayGamma->SetDefaultValueFromString(LexToString(2.2f));
	DisplayGamma->SetShouldApplaySettingsImmediately(true);
	
	GraphicsCategoryCollection->AddChildListData(DisplayGamma);
	
	// Overall quality
	UListDataObject_StringInteger* CreatedOverallQuality = nullptr;
	UListDataObject_StringInteger* OverallQuality = NewObject<UListDataObject_StringInteger>();
	OverallQuality->SetDataID(FName("OverallQuality"));
	OverallQuality->SetDataDisplayName(FText::FromString(TEXT("Overall Quality")));
	OverallQuality->SetDescriptionRichText(GET_DESCRIPTION("OverallQualityDescKey"));
	OverallQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
	OverallQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
	OverallQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
	OverallQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
	OverallQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
	OverallQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
	OverallQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
	OverallQuality->SetShouldApplaySettingsImmediately(true);
	OverallQuality->SetDefaultValueFromString(TEXT("Medium"));
	
	CreatedOverallQuality = OverallQuality;
	GraphicsCategoryCollection->AddChildListData(OverallQuality);
	
	// resolution scale
	UListDataObject_Scalar* ResolutionScale = NewObject<UListDataObject_Scalar>();
	ResolutionScale->SetDataID(FName("ResolutionScale"));
	ResolutionScale->SetDataDisplayName(FText::FromString(TEXT("3D Resolution")));
	ResolutionScale->SetDescriptionRichText(GET_DESCRIPTION("ResolutionScaleDescKey"));
	ResolutionScale->SetDisplayValueRange(TRange<float>(0.f, 1.f));
	ResolutionScale->SetOutputValueRange(TRange<float>(0.f, 1.f));
	ResolutionScale->SetSliderStepSize(0.01f);
	ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);
	ResolutionScale->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
	ResolutionScale->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetResolutionScaleNormalized));
	ResolutionScale->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetResolutionScaleNormalized));
	ResolutionScale->SetShouldApplaySettingsImmediately(true);
	ResolutionScale->SetDefaultValueFromString(LexToString(0.5f));
	
	// changes have to be tracked in both directions
	CreatedOverallQuality->AddEditDependencyData(ResolutionScale);
	
	GraphicsCategoryCollection->AddChildListData(ResolutionScale);
	
	// Global illumination quality
	UListDataObject_StringInteger* GlobalIlluminationQuality = NewObject<UListDataObject_StringInteger>();
	GlobalIlluminationQuality->SetDataID(FName("GlobalIlluminationQuality"));
	GlobalIlluminationQuality->SetDataDisplayName(FText::FromString(TEXT("Global Illumination Quality")));
	GlobalIlluminationQuality->SetDescriptionRichText(GET_DESCRIPTION("GlobalIlluminationQualityDescKey"));
	GlobalIlluminationQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
	GlobalIlluminationQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
	GlobalIlluminationQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
	GlobalIlluminationQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
	GlobalIlluminationQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
	GlobalIlluminationQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetGlobalIlluminationQuality));
	GlobalIlluminationQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetGlobalIlluminationQuality));
	GlobalIlluminationQuality->SetShouldApplaySettingsImmediately(true);
	GlobalIlluminationQuality->SetDefaultValueFromString(TEXT("Medium"));
	GlobalIlluminationQuality->AddEditDependencyData(CreatedOverallQuality);
	
	CreatedOverallQuality->AddEditDependencyData(GlobalIlluminationQuality);
	
	GraphicsCategoryCollection->AddChildListData(GlobalIlluminationQuality);
	
	// Shadow quality
	UListDataObject_StringInteger* ShadowQuality = NewObject<UListDataObject_StringInteger>();
	ShadowQuality->SetDataID(FName("ShadowQuality"));
	ShadowQuality->SetDataDisplayName(FText::FromString(TEXT("Shadow Quality")));
	ShadowQuality->SetDescriptionRichText(GET_DESCRIPTION("ShadowQualityDescKey"));
	ShadowQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
	ShadowQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
	ShadowQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
	ShadowQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
	ShadowQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
	ShadowQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadowQuality));
	ShadowQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadowQuality));
	ShadowQuality->SetShouldApplaySettingsImmediately(true);
	ShadowQuality->SetDefaultValueFromString(TEXT("Medium"));
	ShadowQuality->AddEditDependencyData(CreatedOverallQuality);
	
	CreatedOverallQuality->AddEditDependencyData(ShadowQuality);
	
	GraphicsCategoryCollection->AddChildListData(ShadowQuality);
	
	// Anti Aliasing quality
	UListDataObject_StringInteger* AntiAliasingQuality = NewObject<UListDataObject_StringInteger>();
	AntiAliasingQuality->SetDataID(FName("AntiAliasingQuality"));
	AntiAliasingQuality->SetDataDisplayName(FText::FromString(TEXT("Anti Aliasing Quality")));
	AntiAliasingQuality->SetDescriptionRichText(GET_DESCRIPTION("AntiAliasingDescKey"));
	AntiAliasingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
	AntiAliasingQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
	AntiAliasingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
	AntiAliasingQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
	AntiAliasingQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
	AntiAliasingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAntiAliasingQuality));
	AntiAliasingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAntiAliasingQuality));
	AntiAliasingQuality->SetShouldApplaySettingsImmediately(true);
	AntiAliasingQuality->SetDefaultValueFromString(TEXT("Medium"));
	AntiAliasingQuality->AddEditDependencyData(CreatedOverallQuality);
	
	CreatedOverallQuality->AddEditDependencyData(AntiAliasingQuality);
	
	GraphicsCategoryCollection->AddChildListData(AntiAliasingQuality);
	
	// View distance quality
	UListDataObject_StringInteger* ViewDistanceQuality = NewObject<UListDataObject_StringInteger>();
	ViewDistanceQuality->SetDataID(FName("ViewDistanceQuality"));
	ViewDistanceQuality->SetDataDisplayName(FText::FromString(TEXT("View Distance Quality")));
	ViewDistanceQuality->SetDescriptionRichText(GET_DESCRIPTION("ViewDistanceDescKey"));
	ViewDistanceQuality->AddIntegerOption(0, FText::FromString(TEXT("Near")));
	ViewDistanceQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
	ViewDistanceQuality->AddIntegerOption(2, FText::FromString(TEXT("Far")));
	ViewDistanceQuality->AddIntegerOption(3, FText::FromString(TEXT("Very Far")));
	ViewDistanceQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
	ViewDistanceQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetViewDistanceQuality));
	ViewDistanceQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetViewDistanceQuality));
	ViewDistanceQuality->SetShouldApplaySettingsImmediately(true);
	ViewDistanceQuality->SetDefaultValueFromString(TEXT("Medium"));
	ViewDistanceQuality->AddEditDependencyData(CreatedOverallQuality);
	
	CreatedOverallQuality->AddEditDependencyData(ViewDistanceQuality);
	
	GraphicsCategoryCollection->AddChildListData(ViewDistanceQuality);
	
	// Texture quality
	UListDataObject_StringInteger* TextureQuality = NewObject<UListDataObject_StringInteger>();
	TextureQuality->SetDataID(FName("TextureQuality"));
	TextureQuality->SetDataDisplayName(FText::FromString(TEXT("Texture Quality")));
	TextureQuality->SetDescriptionRichText(GET_DESCRIPTION("TextureQualityDescKey"));
	TextureQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
	TextureQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
	TextureQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
	TextureQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
	TextureQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
	TextureQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetTextureQuality));
	TextureQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetTextureQuality));
	TextureQuality->SetShouldApplaySettingsImmediately(true);
	TextureQuality->SetDefaultValueFromString(TEXT("Medium"));
	TextureQuality->AddEditDependencyData(CreatedOverallQuality);
	
	CreatedOverallQuality->AddEditDependencyData(TextureQuality);
	
	GraphicsCategoryCollection->AddChildListData(TextureQuality);
	
	// Visual Effects quality
	UListDataObject_StringInteger* VisualEffectsQuality = NewObject<UListDataObject_StringInteger>();
	VisualEffectsQuality->SetDataID(FName("VisualEffectsQuality"));
	VisualEffectsQuality->SetDataDisplayName(FText::FromString(TEXT("Visual Effects Quality")));
	VisualEffectsQuality->SetDescriptionRichText(GET_DESCRIPTION("VisualEffectQualityDescKey"));
	VisualEffectsQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
	VisualEffectsQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
	VisualEffectsQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
	VisualEffectsQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
	VisualEffectsQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
	VisualEffectsQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVisualEffectQuality));
	VisualEffectsQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVisualEffectQuality));
	VisualEffectsQuality->SetShouldApplaySettingsImmediately(true);
	VisualEffectsQuality->SetDefaultValueFromString(TEXT("Medium"));
	VisualEffectsQuality->AddEditDependencyData(CreatedOverallQuality);
	
	CreatedOverallQuality->AddEditDependencyData(VisualEffectsQuality);
	
	GraphicsCategoryCollection->AddChildListData(VisualEffectsQuality);
	
	// Refelection quality
	UListDataObject_StringInteger* ReflectionQuality = NewObject<UListDataObject_StringInteger>();
	ReflectionQuality->SetDataID(FName("ReflectionQuality"));
	ReflectionQuality->SetDataDisplayName(FText::FromString(TEXT("Reflection Quality")));
	ReflectionQuality->SetDescriptionRichText(GET_DESCRIPTION("ReflectionQualityDescKey"));
	ReflectionQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
	ReflectionQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
	ReflectionQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
	ReflectionQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
	ReflectionQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
	ReflectionQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetReflectionQuality));
	ReflectionQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetReflectionQuality));
	ReflectionQuality->SetShouldApplaySettingsImmediately(true);
	ReflectionQuality->SetDefaultValueFromString(TEXT("Medium"));
	ReflectionQuality->AddEditDependencyData(CreatedOverallQuality);
	
	CreatedOverallQuality->AddEditDependencyData(ReflectionQuality);
	
	GraphicsCategoryCollection->AddChildListData(ReflectionQuality);
	
	// Post processing quality
	UListDataObject_StringInteger* PostProcessingQuality = NewObject<UListDataObject_StringInteger>();
	PostProcessingQuality->SetDataID(FName("PostProcessingQuality"));
	PostProcessingQuality->SetDataDisplayName(FText::FromString(TEXT("Post Processing Quality")));
	PostProcessingQuality->SetDescriptionRichText(GET_DESCRIPTION("PostProcessingQualityDescKey"));
	PostProcessingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
	PostProcessingQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
	PostProcessingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
	PostProcessingQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
	PostProcessingQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
	PostProcessingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPostProcessingQuality));
	PostProcessingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPostProcessingQuality));
	PostProcessingQuality->SetShouldApplaySettingsImmediately(true);
	PostProcessingQuality->SetDefaultValueFromString(TEXT("Medium"));
	PostProcessingQuality->AddEditDependencyData(CreatedOverallQuality);
	
	CreatedOverallQuality->AddEditDependencyData(PostProcessingQuality);
	
	GraphicsCategoryCollection->AddChildListData(PostProcessingQuality);
	
	VideoTabCollection->AddChildListData(GraphicsCategoryCollection);
	
	// Advanced Grahpics Category
	UListDataObject_Collection* AdvancedGraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
	AdvancedGraphicsCategoryCollection->SetDataID(FName("AdvancedGraphicsCategoryCollection"));
	AdvancedGraphicsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Advanced Graphics")));
	
	// Vertical sync
	UListDataObject_StringBool* VerticalSync = NewObject<UListDataObject_StringBool>();
	VerticalSync->SetDataID(FName("VerticalSync"));
	VerticalSync->SetDataDisplayName(FText::FromString(TEXT("Vertical Sync")));
	VerticalSync->SetDescriptionRichText(GET_DESCRIPTION("VerticalSyncDescKey"));
	VerticalSync->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsVSyncEnabled));
	VerticalSync->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVSyncEnabled));
	VerticalSync->SetFalseAsDefaultValue();
	VerticalSync->SetShouldApplaySettingsImmediately(true);
	
	FOptionsDataEditConditionDescriptor FullScreenOnlyCondition;
	FullScreenOnlyCondition.SetEditConditionFunc([CreatedWindowMode]()
	{
		return CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen;
	});
	FullScreenOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This feature only works if the window mode is set to fullscreen</>"));
	FullScreenOnlyCondition.SetDisabledForcedStringValue(TEXT("false"));
	
	VerticalSync->AddEditCondition(FullScreenOnlyCondition);
	
	AdvancedGraphicsCategoryCollection->AddChildListData(VerticalSync);
	
	// Frame rate limit
	UListDataObject_String* FrameRateLimit = NewObject<UListDataObject_String>();
	FrameRateLimit->SetDataID(FName("FrameRateLimit"));
	FrameRateLimit->SetDataDisplayName(FText::FromString(TEXT("Frame Rate Limit")));
	FrameRateLimit->SetDescriptionRichText(GET_DESCRIPTION("FrameRateLimitDescKey"));
	FrameRateLimit->AddDynamicOption(LexToString(30.f), FText::FromString(TEXT("30 fps")));
	FrameRateLimit->AddDynamicOption(LexToString(60.f), FText::FromString(TEXT("60 fps")));
	FrameRateLimit->AddDynamicOption(LexToString(90.f), FText::FromString(TEXT("90 fps")));
	FrameRateLimit->AddDynamicOption(LexToString(120.f), FText::FromString(TEXT("120 fps")));
	FrameRateLimit->AddDynamicOption(LexToString(0.f), FText::FromString(TEXT("No Limit")));
	FrameRateLimit->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));
	FrameRateLimit->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));
	FrameRateLimit->SetShouldApplaySettingsImmediately(true);
	FrameRateLimit->SetDefaultValueFromString(LexToString(0.f));
	
	AdvancedGraphicsCategoryCollection->AddChildListData(FrameRateLimit);
	
	VideoTabCollection->AddChildListData(AdvancedGraphicsCategoryCollection);
	
	this->RegisteredOptionsTabCollection.Add(VideoTabCollection);
}

// build the content of the key bindings tab 
void UOptionsDataRegistry::InitControllsCollectionTab(ULocalPlayer* InOwningLocalPlayer)
{
	UListDataObject_Collection* ControllsTabCollection = NewObject<UListDataObject_Collection>();
	ControllsTabCollection->SetDataID(FName("ControllsTabCollection"));
	ControllsTabCollection->SetDataDisplayName(FText::FromString(TEXT("Controlls")));
	
	// Keyboard and mouse category
	UListDataObject_Collection* KeyboardMouseCategoryCollection = NewObject<UListDataObject_Collection>();
	KeyboardMouseCategoryCollection->SetDataID(FName("KeyboardMouseCategoryCollection"));
	KeyboardMouseCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Keyboard and Mouse")));
	
	UEnhancedInputLocalPlayerSubsystem* EISubsystem = InOwningLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(EISubsystem);
	
	UEnhancedInputUserSettings* EIUserSettings = EISubsystem->GetUserSettings();
	check(EIUserSettings);
	
	// only query keybaord and mouse interaction, Gamepad options will be ignored
	FPlayerMappableKeyQueryOptions KeyBoardMouseOnly;
	KeyBoardMouseOnly.KeyToMatch = EKeys::S; // any key can be used to activate the whole category
	KeyBoardMouseOnly.bMatchBasicKeyTypes = true;
	
	// FString = name of key mapping setting, ObjectPtr = the actual profile 
	for (const TPair<FString, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& ProfilePair : EIUserSettings->GetAllAvailableKeyProfiles())
	{
		UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;
		check(MappableKeyProfile);
		
		// FName = the ID_* of the input action, MappingRow = the actual mappings per action
		for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
		{
			for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings)
			{
				if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMapping, KeyBoardMouseOnly))
				{
					UListDataObject_KeyRemap* KeyRemapDataObject = NewObject<UListDataObject_KeyRemap>();
					KeyRemapDataObject->SetDataID(KeyMapping.GetMappingName());
					KeyRemapDataObject->SetDataDisplayName(KeyMapping.GetDisplayName());
					KeyRemapDataObject->InitKeyRemapData(EIUserSettings, MappableKeyProfile, ECommonInputType::MouseAndKeyboard, KeyMapping);
					
					KeyboardMouseCategoryCollection->AddChildListData(KeyRemapDataObject);
				}
			}
		}
	}
	
	ControllsTabCollection->AddChildListData(KeyboardMouseCategoryCollection);
	
	// Gamepad category
	UListDataObject_Collection* GamepadCollection = NewObject<UListDataObject_Collection>();
	GamepadCollection->SetDataID(FName("GamepadCollection"));
	GamepadCollection->SetDataDisplayName(FText::FromString(TEXT("Gamepad")));
	
	// only query Gamepad interaction, keybaord and mouse options will be ignored
	FPlayerMappableKeyQueryOptions GempadOnly;
	GempadOnly.KeyToMatch = EKeys::Gamepad_FaceButton_Bottom; // any key can be used to activate the whole category
	GempadOnly.bMatchBasicKeyTypes = true;
	
	for (const TPair<FString, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& ProfilePair : EIUserSettings->GetAllAvailableKeyProfiles())
	{
		UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;
		check(MappableKeyProfile);
		
		// FName = the ID_* of the input action, MappingRow = the actual mappings per action
		for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
		{
			for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings)
			{
				if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMapping, GempadOnly))
				{
					UListDataObject_KeyRemap* KeyRemapDataObject = NewObject<UListDataObject_KeyRemap>();
					KeyRemapDataObject->SetDataID(KeyMapping.GetMappingName());
					KeyRemapDataObject->SetDataDisplayName(KeyMapping.GetDisplayName());
					KeyRemapDataObject->InitKeyRemapData(EIUserSettings, MappableKeyProfile, ECommonInputType::Gamepad, KeyMapping);
					
					GamepadCollection->AddChildListData(KeyRemapDataObject);
				}
			}
		}
	}
	
	ControllsTabCollection->AddChildListData(GamepadCollection);
	
	this->RegisteredOptionsTabCollection.Add(ControllsTabCollection);
}

// get all children recursively for the given element
void UOptionsDataRegistry::FindChildListDataRecursively(const UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutFoundChildListData) const
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