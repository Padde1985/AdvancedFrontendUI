#include "Widgets/Options/OptionsDataRegistry.h"

#include "FrontendFunctionLibrary.h"
#include "FrontendGameplayTags.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"

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
	
	return FoundTabCollection->GetAllChildListData();
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
	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString(TEXT("Audio")));
	
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
