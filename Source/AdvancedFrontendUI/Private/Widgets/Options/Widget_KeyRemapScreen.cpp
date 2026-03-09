#include "Widgets/Options/Widget_KeyRemapScreen.h"
#include "CommonRichTextBlock.h"
#include "Widgets/Options/FKeyRemapScreenInputPreprocessor.h"

// set filter for input type (mouse/keyboard, gamepad, etc)
void UWidget_KeyRemapScreen::SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType)
{
	this->CachedDesiredInputType = InDesiredInputType;
}

// bind callbacks, show widget on screen, and register the input pre processor
void UWidget_KeyRemapScreen::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	this->CachedInputPreprocessor = MakeShared<FKeyRemapScreenInputPreprocessor>(this->CachedDesiredInputType, GetOwningLocalPlayer());
	this->CachedInputPreprocessor->OnInputPreProcessorKeyPressedD.BindUObject(this, &ThisClass::OnValidKeyPressedDetected);
	this->CachedInputPreprocessor->OnInputPreProcessorKeySelectCanceled.BindUObject(this, &ThisClass::OnKeySelectCanceled);
	
	FSlateApplication::Get().RegisterInputPreProcessor(this->CachedInputPreprocessor, -1);
	
	FString InputDeviceName;

	switch (this->CachedDesiredInputType)
	{
	case ECommonInputType::MouseAndKeyboard:
		InputDeviceName = TEXT("Mouse and Keyboard");
		break;
	case ECommonInputType::Gamepad:
		InputDeviceName = TEXT("Gamepad");
		break;
	default:
		break;
	}
	
	const FString DisplayRichMessage = FString::Printf(TEXT("<KeyRemapDefault>Press any</> <KeyRemapHighlight>%s</> <KeyRemapDefault>key. </>"), *InputDeviceName);
	this->CommonRichText_RemapMessage->SetText(FText::FromString(DisplayRichMessage));
}

// unregister the pre processor to enable the engine to react on our key strokes
void UWidget_KeyRemapScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	
	if (this->CachedInputPreprocessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(this->CachedInputPreprocessor);
		this->CachedInputPreprocessor.Reset();
	}
}

// callback for received key input during remapping
void UWidget_KeyRemapScreen::OnValidKeyPressedDetected(const FKey& PressedKey)
{
	this->RequestDeactivateWidget([this, PressedKey]()
	{
		this->OnKeyRemapScreenKeyPressed.ExecuteIfBound(PressedKey);
	});
}

// callback for canceling key remapping
void UWidget_KeyRemapScreen::OnKeySelectCanceled(const FString& CanceledReason)
{
	this->RequestDeactivateWidget([this, CanceledReason]()
	{
		this->OnKeyRemapScreenKeySelectCanceled.ExecuteIfBound(CanceledReason);
	});
}

// deactivate widget and go back to the previous widget screen
void UWidget_KeyRemapScreen::RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback)
{
	// Delay a tick to make sure the input is processed correctly
	FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([PreDeactivateCallback, this](float DeltaTime)
	{
		PreDeactivateCallback();
		
		DeactivateWidget();
		
		return false; // stop ticking
	}));
}
